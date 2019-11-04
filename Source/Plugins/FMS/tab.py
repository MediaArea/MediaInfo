## Copyright (c) MediaArea.net SARL. All Rights Reserved.
 #
 # Use of this source code is governed by a BSD-style license that can
 # be found in the License.html file in the root of the source tree.
 ##

import locale, os, subprocess, csv

try:
  from MediaInfoDLL import *
except ImportError:
  from MediaInfoDLL3 import *

try:
    from urllib import unquote
except ImportError:
    from urllib.parse import unquote

from gi.repository import GObject, Gtk

# generic class for all nautillus-based fms
class MediaInfoTab:
    def __init__(self, filename):
        self.filename = filename
        self.MI = MediaInfo()
        self.boxes = []

        # set mediainfo locale if available
        lang = locale.getdefaultlocale()[0]
        locale_path = os.path.join(os.path.dirname(__file__), "locale")
        self.locale_file = os.path.join(locale_path, lang + ".csv")
        if(not os.path.isfile(self.locale_file)):
            self.locale_file = os.path.join(locale_path, lang.split("_")[0] + ".csv")
            if(not os.path.isfile(self.locale_file)):
                self.locale_file = ""

        if len(self.locale_file) > 0:
            self.MI.Option_Static("Language", "file://{}".format(self.locale_file))

        # parse file
        self.MI.Open(self.filename)

        self.MI.Option("Inform", "Text")
        text = self.MI.Inform()

        self.accepted = False

        for kind in range(Stream.Video, Stream.Max):
            if self.MI.Count_Get(kind) > 0:
                self.accepted = True
                break

        self.create_boxes()
        self.MI.Close()

        # create ui
        self.ui_file = os.path.join(os.path.dirname(__file__), "ui", "ui.glade")
        self.builder = Gtk.Builder()
        self.builder.add_from_file(self.ui_file)

        self.mainWindow = self.builder.get_object("mainWindow")
        self.easyView = self.builder.get_object("easyView")
        self.textView = self.builder.get_object("textView")

        # easyView
        items = {}
        for box in self.boxes:
            if not box['kind'] in items:
                new = Gtk.Box(Gtk.Orientation.HORIZONTAL)
                new.set_homogeneous(True)
                new.show()
                items[box['kind']] = new

                self.easyView.pack_start(new, False, True, 5)

            frame = Gtk.Frame()
            frame.set_label(box['kind'])
            frame.set_label_align(0.02, 0.5)
            frame.show()

            content = Gtk.Box(Gtk.Orientation.HORIZONTAL)
            content.set_homogeneous(True)
            content.show()

            for column in box['columns']:
                label = Gtk.Label()
                label.set_text(column)
                label.set_xalign(0.0)
                label.set_yalign(0.0)
                label.set_line_wrap(True)
                label.show()

                content.pack_start(label, True, True, 2)

            frame.add(content)
            items[box['kind']].pack_start(frame, True, True, 5)

        # textView
        self.textView.get_buffer().set_text(text)

        # link to MediaInfo
        if any(os.access(os.path.join(path, "mediainfo-gui"), os.X_OK) for path in os.environ['PATH'].split(os.pathsep)):
            button = Gtk.Button()
            button.set_label("Open in MediaInfo")
            button.connect("clicked", self.mediainfo_open)
            button.show()

            self.mainWindow.pack_start(button, False, False, 5)

    def summary_get(self, kind, pos):
        self.MI.Option("Inform", "Summary")
        return self.MI.Get(kind, pos, "Inform")

    def translated_name_get (self, name):
        if len(self.locale_file) > 0:
            with open(self.locale_file, 'r') as locale:
                data = csv.DictReader(locale,fieldnames=["name", "value"], restkey="extra", restval="", delimiter=";")
                for line in data:
                    if line.get("name") == name:
                        return line.get("value", name)

        return name

    def name_get(self, kind):
        if kind == Stream.General: return self.translated_name_get("General")
        elif kind == Stream.Video: return self.translated_name_get("Video")
        elif kind == Stream.Audio: return self.translated_name_get("Audio")
        elif kind == Stream.Text: return self.translated_name_get("Text")
        elif kind == Stream.Other: return self.translated_name_get("Other")
        elif kind == Stream.Image: return self.translated_name_get("Image")
        elif kind == Stream.Menu: return self.translated_name_get("Menu")
        else: return "Unknown"

    def boxes_count_get(self, kind):
        if kind == Stream.General: return 1
        elif kind == Stream.Video: return 1
        elif kind == Stream.Audio: return 2
        elif kind == Stream.Text: return 3
        elif kind == Stream.Image: return 1
        else: return 0

    def lines_count_get(self, kind):
        if kind == Stream.General: return 5
        elif kind == Stream.Video: return 2
        elif kind == Stream.Audio: return 2
        elif kind == Stream.Text: return 2
        elif kind == Stream.Image: return 2
        else: return 0

    def create_boxes(self):
        for kind in range(Stream.General, Stream.Max):
            count = self.boxes_count_get(kind)
            if count == 0: continue
            for pos in range(0, count):
                box = self.create_box(kind, pos)
                if len(box) != 0:
                    self.boxes.append(box)

    def create_box(self, kind, pos):
        content = {}
        summary = self.summary_get(kind, pos)

        if(kind == Stream.General):
            for index in range(Stream.Video, Stream.Max):
                if self.MI.Count_Get(index) == 0:
                    continue

                z1 = self.MI.Get(index, 0, "StreamKind", Info.Measure) + "StreamCount"
                z2 = " " + self.MI.Get(index, 0, "StreamKind", Info.Text) + " stream"
                z3 = self.MI.Get(index, 0, "StreamKind", Info.Text) + "_Format_WithHint_List"

                summary += "\n"
                summary += self.MI.Get(index, 0, z1) + z2 + ": " + self.MI.Get(0, 0, z3)

            content["columns"] = [summary, self.tags_get_general()]
            content["kind"] = self.name_get(kind)
        elif len(summary) > 0 and self.lines_count_get(kind) > 0:
            content["columns"] = [summary]
            content["kind"] = self.name_get(kind)

        return content

    def tags_get_general(self):
        toReturn = ""

        pos = 94 # title_pos
        max = self.MI.Count_Get(Stream.General, 0)
        lines = 0
        while pos < max:
            z1 = self.MI.GetI(Stream.General, 0, pos)
            options = self.MI.GetI(Stream.General, 0, pos, Info.Options)

            if len(options) > InfoOption.ShowInInform and options[InfoOption.ShowInInform] == 'Y' and len(z1) > 0:
                z2 = self.MI.GetI(Stream.General, 0, pos, Info.Name_Text)
                if len(z2) == 0:
                    z2 = self.MI.GetI(Stream.General, 0, pos, Info.Name)
                z2 += ": " + z1

                if len(toReturn) > 0:
                    toReturn += "\n"
                toReturn += z2

                lines += 1
                if lines >= self.lines_count_get(Stream.General):
                    break

            pos += 1

        return toReturn

    def mediainfo_open(self, widget):
        subprocess.Popen(["mediainfo-gui", self.filename])
