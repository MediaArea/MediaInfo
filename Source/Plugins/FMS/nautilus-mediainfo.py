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

from gi.repository import GObject, Gtk, Nautilus

class MediainfoPropertyPage(GObject.GObject, Nautilus.PropertyPageProvider):
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

        self.label = Gtk.Label('MediaInfo')
        self.label.show()

        return Nautilus.PropertyPage(name="NautilusPython::mediainfo", label=self.label, page=self.tab.mainWindow),
