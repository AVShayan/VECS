pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    // This allows the build engines to merge repositories instead of crashing!
    repositoriesMode.set(RepositoriesMode.PREFER_SETTINGS) 
    repositories {
        google()
        mavenCentral()
    }
}
rootProject.name = "VECS"
include(":app")