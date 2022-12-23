::set dname_in=E:\ndpi\C01KNJTHI00389S01.ndpi
set dname_in=E:\ndpi\
set dname_in_ndpa=E:\ndpi\B1-B3_20221221\
set dname_out=.\output\
set level=2

mkdir %dname_out%

.\bin\Release\test_slide2.exe %dname_in% %dname_in_ndpa% %dname_out% %level%

