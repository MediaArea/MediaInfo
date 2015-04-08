#!/usr/bin/env python
import sys
import os
import urllib2
from urllib import urlencode
import cookielib
import re

#found at http://thebigbrowser.blogspot.com/2010/08/use-google-translate-from-console.html

# The google translate API can be found here (***NOT OPERATIONAL SINCE DECEMBER 2011***):
# http://code.google.com/apis/ajaxlanguage/documentation/#Examples

# Language codes are listed here:
#http://code.google.com/apis/ajaxlanguage/documentation/reference.html#LangNameArray

if len(sys.argv) < 3:
    name = os.path.basename(sys.argv[0])
    print '''
Usage:
    %s en es lovely spam
    %s es en < file.txt

Available language codes are listed here:
http://code.google.com/apis/ajaxlanguage/documentation/reference.html#LangNameArray

''' % (name,name)
    sys.exit(-1)

## hack to be able to display UTF-8 in Windows console

def fix_win32_console():
    ## set utf8 console
    if not sys.stdin.encoding == 'cp65001':
        os.system('chcp 65001 > nul')
    class UniStream(object):
        __slots__= "fileno", "softspace",
        def __init__(self, fileobject):
            self.fileno= fileobject.fileno()
            self.softspace = False
        def write(self, text):
            if isinstance(text, unicode):
                os.write(self.fileno, text.encode("utf_8"))
            else:
                os.write(self.fileno, text)
    sys.stdout= UniStream(sys.stdout)
    sys.stderr= UniStream(sys.stderr)

if sys.platform == "win32":
    fix_win32_console()

lang1=sys.argv[1]
lang2=sys.argv[2]

if len(sys.argv) > 3:
    text=' '.join(sys.argv[3:])
else:
    text=sys.stdin.read()

base_url='http://translate.google.com.br/translate_a/t'
# sample browser request
#http://translate.google.com/translate_a/t?client=t&text=col&hl=en&sl=en&tl=es&multires=1&otf=2&ssel=4&tsel=0&sc=1
params=urlencode({'client':'t',
    'text':text,
    'hl':'en',
    'sl':lang1,
    'tl':lang2,
    'otf':2,
    'multires':1,
    'ssel':0,
    'tsel':0,
    'sc':1,
    })

url=base_url + '?' + params

cookiejar = cookielib.CookieJar()
opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cookiejar))
opener.addheaders = [('User-agent', 'Mozilla/5.0 (Windows NT 5.1) AppleWebKit/535.19 (KHTML, like Gecko) Chrome/18.0.1018.0 Safari/535.19'),
                    ('Referer', 'http://translate.google.com/')
]
response = opener.open(url)
translation=response.read()
matcher = re.search('\[\[\["(?P<human_readable_chunk>[^")]*)', translation)
sys.stdout.write(matcher.group('human_readable_chunk'))