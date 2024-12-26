# IchigoJam P

## 環境構築
- **CMake**と**GCC**をインストールする  

- 以下のライブラリを`IchigoJam_P`ディレクトリ直下に置く  
    - **IchigoJam_BASIC**: 1つ上の階層にある`IchigoJam_BASIC`ディレクトリをコピーする  
    - **pico-sdk**: https://github.com/NaturalStyle/pico-sdk.git `196662b`のコミットを使用(別のコミットを使用すると正しく動作しない)  
    - **pico-extras**: https://github.com/raspberrypi/pico-extras.git 開発時は`09c64d5`のコミットを使用  
    - **PicoDVI**: https://github.com/mlorenzati/PicoDVI.git `579eecc`のコミットを使用(別のコミットを使用すると正しく動作しない)  

- 以下のパスを通す  
    ```sh
    PICO_SDK_PATH=foo/IchigoJam_P/pico-sdk  
    PICO_EXTRAS_PATH=foo/IchigoJam_P/pico-extras  
    ```

詳細は以下を参照  
[Raspberry Pi Picoをセットアップしよう](https://datasheets.raspberrypi.com/pico/getting-started-with-pico-JP.pdf)  

## ビルド
```sh
cd IchigoJam_P  
mkdir build  
cd build  
cmake ..  
make  
```

`IchigoJam_P.uf2`ファイルが作成できたらpicoに書き込む  

## 動作がおかしくなったら
`build`ディレクトリを消して作り直してみる  
特に映像の出力が正常に行われなくなることが多い  

## TODO
高解像度に対応する(TVでも使えるようにするため)  

## 商標 / Trademark
IchigoJamは、株式会社jig.jpの登録商標です(IchigoJam is a registered trademark of jig.jp co., ltd.)  
Raspberry Pi は、Raspberry Pi財団の登録商標です(Raspberry Pi is a registered trademark of the Raspberry Pi Foundation)  

株式会社ナチュラルスタイル  
2024/12/25 作成  