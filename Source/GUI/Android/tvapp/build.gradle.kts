import org.jetbrains.kotlin.gradle.dsl.JvmTarget

plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
    id("org.jetbrains.kotlin.plugin.compose")
}

android {
    namespace = "net.mediaarea.mediainfo.tv"
    compileSdk {
        version = release(36)
    }

    defaultConfig {
        applicationId = "net.mediaarea.mediainfo.tv"
        minSdk = 23
        targetSdk = 36
        versionCode = 1
        versionName = "25.10"
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
                arguments("-DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON")
            }
        }
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
    buildFeatures {
        buildConfig = true
        compose = true
    }
}

dependencies {
    implementation("androidx.core:core-ktx:1.17.0")
    implementation("androidx.appcompat:appcompat:1.7.1")
    implementation(platform("androidx.compose:compose-bom:2025.12.01"))
    implementation("androidx.compose.material:material-icons-extended")
    implementation("androidx.compose.material3:material3")
    implementation("androidx.compose.ui:ui")
    implementation("androidx.compose.ui:ui-graphics")
    implementation("androidx.compose.ui:ui-tooling-preview")
    implementation("androidx.datastore:datastore-preferences:1.2.0")
    implementation("androidx.tv:tv-foundation:1.0.0-alpha12")
    implementation("androidx.tv:tv-material:1.0.1")
    implementation("androidx.lifecycle:lifecycle-process:2.10.0")
    implementation("androidx.lifecycle:lifecycle-runtime-ktx:2.10.0")
    implementation("androidx.activity:activity-compose:1.12.2")
    implementation("androidx.navigation:navigation-compose:2.9.6")
    implementation("com.google.accompanist:accompanist-permissions:0.37.3")
    androidTestImplementation(platform("androidx.compose:compose-bom:2025.12.01"))
    androidTestImplementation("androidx.compose.ui:ui-test-junit4")
    debugImplementation("androidx.compose.ui:ui-tooling")
    debugImplementation("androidx.compose.ui:ui-test-manifest")
}
