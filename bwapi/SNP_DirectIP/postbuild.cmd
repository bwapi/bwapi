copy /Y /B "%1" + "SNP\caps.mpq" "%1.snp"

if not exist custom_postbuild.cmd goto Lend
call custom_postbuild.cmd %1.snp
:Lend
