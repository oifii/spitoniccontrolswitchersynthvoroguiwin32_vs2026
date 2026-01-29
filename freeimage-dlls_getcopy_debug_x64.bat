:LABEL1
rem 2021nov30 prebuilt freeimage3180_vs2017 instead
rem 32bit
rem 32bit
rem 32bit
copy ..\lib-src\freeimage3180_vs2017\Dist\x32\FreeImage.dll FreeImage.dll
copy ..\lib-src\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlusd.dll FreeImagePlusd.dll
copy ..\lib-src\freeimage3180_vs2017\Dist\x32\FreeImage.dll debug\FreeImage.dll
copy ..\lib-src\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlusd.dll debug\FreeImagePlusd.dll
rem release dlls
copy ..\lib-src\freeimage3180_vs2017\Dist\x32\FreeImage.dll release\FreeImage.dll
copy ..\lib-src\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlus.dll release\FreeImagePlus.dll


rem 64bit
rem 64bit
rem 64bit
copy ..\lib-src\freeimage3180_vs2017\Dist\x64\FreeImage.dll FreeImage.dll
copy ..\lib-src\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x64\FreeImagePlus.dll FreeImagePlus.dll
copy ..\lib-src\freeimage3180_vs2017\Dist\x64\FreeImage.dll x64\debug\FreeImage.dll
copy ..\lib-src\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x64\FreeImagePlus.dll x64\debug\FreeImagePlus.dll
rem release dlls
copy ..\lib-src\freeimage3180_vs2017\Dist\x64\FreeImage.dll x64\release\FreeImage.dll
copy ..\lib-src\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x64\FreeImagePlus.dll x64\release\FreeImagePlus.dll
exit

rem :LABEL2
rem debug dlls
rem 2021nov29 freeimage_vs2017 does not compile totally
rem copy ..\lib-src\freeimage_vs2017\Dist\x32\FreeImage.dll FreeImage.dll
rem copy ..\lib-src\freeimage_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlus.dll FreeImagePlus.dll
rem copy ..\lib-src\freeimage_vs2017\Dist\x32\FreeImage.dll debug\FreeImage.dll
rem copy ..\lib-src\freeimage_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlus.dll debug\FreeImagePlus.dll
rem release dlls
rem copy ..\lib-src\freeimage_vs2017\Dist\x32\FreeImage.dll release\FreeImage.dll
rem copy ..\lib-src\freeimage_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlus.dll release\FreeImagePlus.dll
rem exit

