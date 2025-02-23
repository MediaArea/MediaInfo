if not exist nuget.exe (
    curl -O https://dist.nuget.org/win-x86-commandline/latest/nuget.exe
)
nuget install Microsoft.Web.WebView2 -OutputDirectory packages -ExcludeVersion
