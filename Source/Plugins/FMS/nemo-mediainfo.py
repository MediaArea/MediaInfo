## Copyright (c) MediaArea.net SARL. All Rights Reserved.
 #
 # Use of this source code is governed by a BSD-style license that can
 # be found in the License.html file in the root of the source tree.
 ##

import locale, os, subprocess
from mediainfo.tab import *

try:
    from urllib import unquote
except ImportError:
    from urllib.parse import unquote

from gi.repository import GObject, Gtk, Nemo

class MediainfoPropertyPage(GObject.GObject, Nemo.PropertyPageProvider, Nemo.NameAndDescProvider):
    def get_property_pages(self, files):
        # TODO: handle multiples files
        if len(files) != 1:
          return

        file = files[0]
        if file.get_uri_scheme() != 'file':
            return

        if file.is_directory():
            return

        filename = unquote(file.get_uri()[7:])

        try:
            filename = filename.decode("utf-8")
        except:
            pass

        self.tab = MediaInfoTab(filename)

        if not self.tab.accepted:
            return

        self.property_label = Gtk.Label('MediaInfo')
        self.property_label.show()

        return Nemo.PropertyPage(name="NemoPython::mediainfo", label=self.property_label, page=self.tab.mainWindow),

    def get_name_and_desc(self):
        return [("Nemo MediaInfo Tab:::Dispay technical informations about media files")]
