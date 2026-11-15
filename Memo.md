# インストール、ビルド、起動
## インストール
```
sudo apt-get update
sudo apt-get install build-essential gcc g++ make cmake git python3 python3-pip uuid-dev nasm acpica-tools qemu-system-x86 ovmf mtools gnu-efi
```
```
cp /usr/share/OVMF/OVMF_VARS_4M.fd ./OVMF_VARS.fd
```

## ビルド
* Clionは、ビルドボタンを押すだけ!

## 起動
* コントロール + Alt + Gで戻る
```
qemu-system-x86_64   -machine q35   -m 256M   -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE_4M.fd   -drive if=pflash,format=raw,file=./OVMF_VARS.fd   -drive format=raw,file=fat:rw:esp
```

## その他
* /usr/include/efi
* /usr/include/efi/x86_64


# 
* EFI_ERROR ：　エラー用のマクロ。Windowsで言う、FAILEDみたいなやつ？
