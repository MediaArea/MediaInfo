/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

import Foundation

// convert String to null-terminated wchar_t C array
extension String {
    func withWideChars<Result>(_ body: (UnsafePointer<wchar_t>) -> Result) -> Result {
        let u32 = unicodeScalars.map { wchar_t(bitPattern: $0.value) } + [0]
        return u32.withUnsafeBufferPointer { body($0.baseAddress!) }
    }
}

extension Notification.Name {
    static let darkModeEnabled = Notification.Name("net.mediaarea.mediainfo.ios.notifications.darkModeEnabled")
    static let darkModeDisabled = Notification.Name("net.mediaarea.mediainfo.ios.notifications.darkModeDisabled")
}

class Core {
    struct ReportView {
        var name: String
        var desc: String
        var mime: String
        var exportable: Bool
    }

    struct States: OptionSet {
        let rawValue: Int
        static let Accepted = States(rawValue: 1 << 0)
        static let Filled  = States(rawValue: 1 << 1)
        static let Updated = States(rawValue: 1 << 2)
        static let Finalized = States(rawValue: 1 << 3)
    }

    static let shared = Core()

    var mi: UnsafeMutableRawPointer

    var darkMode: Bool {
        get {
            return UserDefaults.standard.bool(forKey: "DarkMode")
        }
        set {
            UserDefaults.standard.set(newValue, forKey: "DarkMode")
            NotificationCenter.default.post(name: newValue ? .darkModeEnabled : .darkModeDisabled, object: nil)
        }
    }

    var userLocale: Bool {
        get {
            if SubscriptionManager.shared.subscriptionActive && UserDefaults.standard.object(forKey: "UserLocale") != nil {
                  return UserDefaults.standard.bool(forKey: "UserLocale")
            }
            else {
                return true
            }
        }
        set {
            UserDefaults.standard.set(newValue, forKey: "UserLocale")
        }
    }

    var locale: String {
        get {
            if userLocale {
                if let path = Bundle.main.path(forResource: "lang", ofType: "csv") {
                    if let content = try? String.init(contentsOfFile: path, encoding: .utf8) {
                        return content
                    }
                }
            }
            return ""
        }
    }
    var views: Array<ReportView> {
        get {
            var views: Array<ReportView> = Array<ReportView>()

            "Info_OutputFormats_CSV".withWideChars { wName in
                "".withWideChars { wValue in
                    let viewsCsv: String = wideStringToString(wideString: MediaInfo_Option(mi, wName, wValue))

                    for viewCsv: String in viewsCsv.components(separatedBy: "\n") {
                        let view: Array<String> = viewCsv.components(separatedBy: ",")
                        if view.count > 2 {
                                views.append(Core.ReportView(name: view[0], desc: view[1], mime: view[2], exportable: true))
                        }
                    }
                }
            }
            return views
        }
    }
    var version: String {
        get {
            var version: String = ""

            "Info_Version".withWideChars { wName in
                "".withWideChars { wValue in
                    version = wideStringToString(wideString: MediaInfo_Option(mi, wName, wValue))
                    version = version.replacingOccurrences(of: "MediaInfoLib - v", with: "")
                }
            }
            return version
        }
    }

    init() {
        mi = MediaInfo_New()
        var _ = locale
    }

    deinit {
        MediaInfo_Delete(mi)
    }

    func getExtension(view: String) -> String {
        for item in views {
            if item.name == view {
                switch item.mime {
                case "text/html":
                    return "html"
                case "text/xml":
                    return "xml"
                case "text/json":
                    return "json"
                case "text/plain":
                    return "txt"
                default:
                    return "txt"
                }
            }
        }
        return "txt"
    }

    func wideStringToString(wideString: UnsafePointer<wchar_t>) -> String {
        return NSString(bytes: UnsafePointer(wideString), length: wcslen(wideString) * MemoryLayout<wchar_t>.size, encoding: String.Encoding.utf32LittleEndian.rawValue) as String? ?? ""
    }

    func createReport(url: URL, name: String) throws -> Array<MediaInfo_int8u> {
        _ = "Inform".withWideChars { wName in
            "MIXML".withWideChars { wValue in
                MediaInfo_Option(mi, wName, wValue)
            }
        }

        _ = "Input_Compressed".withWideChars { wName in
            "".withWideChars { wValue in
                MediaInfo_Option(mi, wName, wValue)
            }
        }

        _ = "Inform_Compress".withWideChars { wName in
            "zlib+base64".withWideChars { wValue in
                MediaInfo_Option(mi, wName, wValue)
            }
        }

        let file: FileHandle = try FileHandle(forReadingFrom: url)
        let fileSize = file.seekToEndOfFile()

        file.seek(toFileOffset: 0)

        _ = "File_FileName".withWideChars { wName in
            name.withWideChars { wValue in
                MediaInfo_Option(mi, wName, wValue)
            }
        }

        MediaInfo_Open_Buffer_Init(mi, fileSize, 0)

        while true {
            let data: Data = file.readData(ofLength: 1024 * 1024)
            var state: States = States(rawValue: 0)
            let size = data.count

            let buffer: UnsafeMutablePointer<MediaInfo_int8u> = UnsafeMutablePointer<MediaInfo_int8u>.allocate(capacity: size)
            data.copyBytes(to: buffer, count: size)
            state = States(rawValue: MediaInfo_Open_Buffer_Continue(mi, buffer, size))
            buffer.deallocate()

            if state == States.Finalized {
                break
            }

            // test if there is a MediaInfo request to go elsewhere
            let seekTo: MediaInfo_int64u = MediaInfo_Open_Buffer_Continue_GoTo_Get(mi)

            if seekTo != MediaInfo_int64u.max {
                file.seek(toFileOffset: seekTo)
                MediaInfo_Open_Buffer_Init(mi, fileSize, file.offsetInFile) // inform MediaInfo we have seek
                continue
            }

            // EOF and no seekTo request
            if data.count == 0 {
                break
            }
        }

        MediaInfo_Open_Buffer_Finalize(mi)

        file.closeFile()

        return [MediaInfo_int8u](wideStringToString(wideString: MediaInfo_Inform(mi, 0)).utf8)
    }

    func convertReport(report: Array<MediaInfo_int8u>, format: String, export: Bool = false) -> String {
        var output: String = ""

        _ = "Inform".withWideChars { wName in
            format.withWideChars { wValue in
                MediaInfo_Option(mi, wName, wValue)
            }
        }

        _ = "Inform_Compress".withWideChars { wName in
            "".withWideChars { wValue in
                MediaInfo_Option(mi, wName, wValue)
            }
        }

        _ = "Input_Compressed".withWideChars { wName in
            "zlib+base64".withWideChars { wValue in
                MediaInfo_Option(mi, wName, wValue)
            }
        }

        if format == "Text" && export == false {
            var value = locale.replacingOccurrences(of: "  Config_Text_ColumnSize;40", with: "  Config_Text_ColumnSize;25")
            if value.isEmpty {
                value = "  Config_Text_ColumnSize;25"
            }
            _ = "Language".withWideChars { wName in
                value.withWideChars { wValue in
                    MediaInfo_Option(mi, wName, wValue)
                }
            }
        }
        else {
            _ = "Language".withWideChars { wName in
                locale.withWideChars { wValue in
                    MediaInfo_Option(mi, wName, wValue)
                }
            }
        }

        let cArray: UnsafeMutablePointer<MediaInfo_int8u> = UnsafeMutablePointer(mutating: report)

        MediaInfo_Open_Buffer_Init(mi, MediaInfo_int64u(report.count), MediaInfo_int64u(0))
        MediaInfo_Open_Buffer_Continue(mi, cArray, report.count)
        MediaInfo_Open_Buffer_Finalize(mi)

        output = wideStringToString(wideString: MediaInfo_Inform(mi, 0))

        MediaInfo_Close(mi)

        return output
    }
}
