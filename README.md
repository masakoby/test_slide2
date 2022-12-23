# test_slide2
- Purpose of the program

アノテーションした細胞のうち、AI検出結果が見落としているものを分析するためのプログラム。  
アノテーションした細胞とAI検出結果は、ndpa形式で保存されている。  


- Windows settings:   
.\vcpkg.exe install opencv:x64-windows  
.\vcpkg.exe install xerces-c:x64-windows  

- Other download source codes  
https://github.com/masakoby/ds.git  
https://github.com/masakoby/xml.git  
https://github.com/masakoby/cv.git  
https://github.com/masakoby/common.git  

- folder positions
<pre>
.
├──test_slide2
├──common
├──cv
├──ds
└──xml
</pre>

- input data1:   
ndpiファイルが格納されているフォルダへのパスをバッチファイルの第一引数で指定する。　　

- input data2:   
分類抽出プログラムから出力されるデータは下記のようなフォルダ構造で管理されている。  
<pre>
.
├──all
├──anno_check
├──correct_check
├──nbd_pickups
└──top30
</pre>
allフォルダ内にあるndpaファイルには、細胞検出結果と病理医のアノテーション結果が格納されている。これを読み込んで、アノテーション位置に検出した細胞があるかどうかで判断する。
バッチファイルの第２引数にallフォルダへのパスを指定しておく。

- input data3:   
case_information.txt
ndpiファイル名と診断名（NILM, LSIL, HSIL等）の対応付けファイルをCSV形式で作り、ndpiフォルダに置いておく。　　

- output data  
outputフォルダ内に、all_cells.csvとndpi_summary.csvが出力される。  
all_cells.csv  
G列：doctor_diagは、病理医がアノテーションした細胞の種類  
0=SILは確定だが、LSILかHSILか迷う細胞  
1=LSILを疑う細胞  
2=HSILを疑う細胞  　


ndpi_summary.csv  
C列：　病理医がアノテーションした細胞数  
D列：　細胞抽出結果で抽出できた細胞数  
E列：　細胞分類結果で抽出できた細胞数  　


