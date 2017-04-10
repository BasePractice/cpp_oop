@echo off
REG ADD "HKCU\Software\Google\Chrome\NativeMessagingHosts\ru.pastor.nm.example" /ve /t REG_SZ /d "C:\work\base_practice\cpp_oop\06_cpp_stl\nmh-manifest.json" /f
REG ADD "HKLM\Software\Mozilla\NativeMessagingHosts\ru.pastor.nm.example"       /ve /t REG_SZ /d "C:\work\base_practice\cpp_oop\06_cpp_stl\nmh-manifest-firefox.json" /f