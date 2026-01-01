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
                arguments(
                    "-DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON",
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
    implementation("androidx.core:core-ktx:1.18.0")
    implementation("androidx.appcompat:appcompat:1.7.1")
    implementation(platform("androidx.compose:compose-bom:2026.03.00"))
    implementation("androidx.compose.material:material-icons-extended")
    implementation("androidx.compose.material3:material3")
    implementation("androidx.compose.ui:ui")
    implementation("androidx.compose.ui:ui-graphics")
    implementation("androidx.compose.ui:ui-tooling-preview")
    implementation("androidx.datastore:datastore-preferences:1.2.1")
    implementation("androidx.tv:tv-foundation:1.0.0-beta01")
    implementation("androidx.tv:tv-material:1.0.1")
    implementation("androidx.lifecycle:lifecycle-process:2.10.0")
    implementation("androidx.lifecycle:lifecycle-runtime-ktx:2.10.0")
    implementation("androidx.activity:activity-compose:1.13.0")
    implementation("androidx.navigation:navigation-compose:2.9.7")
    implementation("com.google.accompanist:accompanist-permissions:0.37.3")
    androidTestImplementation(platform("androidx.compose:compose-bom:2026.03.00"))
    androidTestImplementation("androidx.compose.ui:ui-test-junit4")
    debugImplementation("androidx.compose.ui:ui-tooling")
    debugImplementation("androidx.compose.ui:ui-test-manifest")
}
