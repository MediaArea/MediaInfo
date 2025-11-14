set /P CodeSigningCertificatePass= < %USERPROFILE%\CodeSigningCertificate.pass
if "%NOSIGN%"=="" (
    signtool.exe sign /f %USERPROFILE%\CodeSigningCertificate.p12 /p %CodeSigningCertificatePass% /fd sha256 /v /tr http://timestamp.acs.microsoft.com /td sha256 /d %2 /du http://mediaarea.net %1
)
set CodeSigningCertificatePass=
