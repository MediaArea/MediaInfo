import org.jetbrains.kotlin.gradle.dsl.JvmTarget

plugins {
    id("com.android.library")
    id("org.jetbrains.kotlin.android")
}

android {
    namespace = "net.mediaarea.mediainfo.lib"
    compileSdk {
        version = release(36)
    }

    defaultConfig {
        minSdk = 21

        @Suppress("UnstableApiUsage")
        externalNativeBuild {
            cmake {
                cppFlags(
                    "-DUNICODE",
                    "-D_UNICODE",
                    "-DMEDIAINFO_ADVANCED_NO",
                    "-DMEDIAINFO_REFERENCES_NO",
                    "-DMEDIAINFO_FILTER_NO",
                    "-DMEDIAINFO_DUPLICATE_NO",
                    "-DMEDIAINFO_MACROBLOCKS_NO",
                    "-DMEDIAINFO_TRACE_NO",
                    "-DMEDIAINFO_TRACE_FFV1CONTENT_NO",
                    "-DMEDIAINFO_IBI_NO",
                    "-DMEDIAINFO_DIRECTORY_NO",
                    "-DMEDIAINFO_LIBCURL_NO",
                    "-DMEDIAINFO_LIBMMS_NO",
                    "-DMEDIAINFO_DVDIF_ANALYZE_NO",
                    "-DMEDIAINFO_MPEGTS_DUPLICATE_NO",
                    "-DMEDIAINFO_READTHREAD_NO",
                    "-DMEDIAINFO_MD5_NO",
                    "-DMEDIAINFO_SHA1_NO",
                    "-DMEDIAINFO_SHA2_NO",
                    "-DMEDIAINFO_EVENTS_NO",
                    "-DMEDIAINFO_DEMUX_NO",
                    "-DMEDIAINFO_AES_NO",
                    "-DMEDIAINFO_FIXITY_NO",
                    "-DMEDIAINFO_READER_NO",
                    "-DMEDIAINFO_NEXTPACKET_NO"
                )
                cFlags(
                    "-Wno-qualified-void-return-type" // warning in aes_via_ace.h for x86 arch
                )
                arguments("-DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON")
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
    kotlin {
        compilerOptions {
            jvmTarget = JvmTarget.JVM_21
        }
    }
}

dependencies {

}
