avr-objcopy -S  -j .text -j .data -O binary vario/Release/vario.elf vario/Release/vario.bin
openssl base64 -in vario/Release/vario.bin  -out vario/Release/vario.ebin
