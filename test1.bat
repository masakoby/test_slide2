::set dname_in=E:\ndpi\C01KNJTHI00389S01.ndpi
set dname_in=F:\ndpi\
set dname_out=.\output\
set level=2

mkdir %dname_out%

.\bin\Release\test_slide2.exe %dname_in% %dname_out% %level%

