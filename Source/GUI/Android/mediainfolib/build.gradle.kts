plugins {
    id("com.android.library")
}

android {
    namespace = "net.mediaarea.mediainfo.lib"
    compileSdk {
        version = release(37)
    }

    defaultConfig {
        minSdk = 21

        @Suppress("UnstableApiUsage")
        externalNativeBuild {
            cmake {
                arguments(
                    "-DENABLE_UNICODE=ON",
                    "-DMEDIAINFO_ADVANCED=OFF",
                    "-DMEDIAINFO_AES=OFF",
                    "-DMEDIAINFO_DEMUX=OFF",
                    "-DMEDIAINFO_DIRECTORY=OFF",
                    "-DMEDIAINFO_DUPLICATE=OFF",
                    "-DMEDIAINFO_DVDIF_ANALYZE=OFF",
                    "-DMEDIAINFO_EVENTS=OFF",
                    "-DMEDIAINFO_FILTER=OFF",
                    "-DMEDIAINFO_FIXITY=OFF",
                    "-DMEDIAINFO_GRAPH=OFF",
                    "-DMEDIAINFO_IBI=OFF",
                    "-DMEDIAINFO_LIBCURL=OFF",
                    "-DMEDIAINFO_LIBMMS=OFF",
                    "-DMEDIAINFO_MACROBLOCKS=OFF",
                    "-DMEDIAINFO_MD5=OFF",
                    "-DMEDIAINFO_NEXTPACKET=OFF",
                    "-DMEDIAINFO_READER=OFF",
                    "-DMEDIAINFO_READTHREAD=OFF",
                    "-DMEDIAINFO_REFERENCES=OFF",
                    "-DMEDIAINFO_SHA1=OFF",
                    "-DMEDIAINFO_SHA2=OFF",
                    "-DMEDIAINFO_TRACE=OFF",
                    "-DMEDIAINFO_TRACE_FFV1CONTENT=OFF"
                )
            }
        }

        consumerProguardFiles("consumer-rules.pro")
    }

    externalNativeBuild {
        cmake {
            path = file("CMakeLists.txt")
        }
    }
    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_21
        targetCompatibility = JavaVersion.VERSION_21
    }
    packaging {
        jniLibs {
            keepDebugSymbols.add("**/*.so")
        }
    }
}

dependencies {

}
