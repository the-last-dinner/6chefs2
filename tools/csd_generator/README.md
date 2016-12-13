# csd_generator
USAGE
1. csdファイルを作りたいplistとpngの準備
targetsディレクトリにcsd作成対象のplistとpngを準備しておく

2. csd_generatorを実行
引数がなければ、デフォルトでnadeshiko.csdが見本になり、
引数で指定すればそのキャラのcsdファイルが見本となる
```
$ sh csd_generator.sh
```

or 

```
$ sh csd_generator.sh ${characterName}
```

3. csv_moverを実行
targetsディレクトリに正しくcsdファイルが生成されていたら
```
$ sh csd_mover.sh
```

4. cocosstudioを開く
File > import > importResources
で対象のcocosstudio/character/のファイルをimportする

5. Resources/charactersディレクトリにcsbファイルを書き出す
問題がなければ上部の書き出しボタンで目的のディレクトリにcsbファイルを生成
