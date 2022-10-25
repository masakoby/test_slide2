set dname_in=E:\ndpi\C01KNJTHI00389S01.ndpi
set dname_out=.\output\
set level=3

mkdir %dname_out%

.\bin\Release\test_slide2.exe %dname_in% %dname_out%a.jpg %level%

