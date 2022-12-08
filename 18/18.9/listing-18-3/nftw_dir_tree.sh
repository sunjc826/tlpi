mkdir tmpdir
touch tmpdir/a tmpdir/b
ln -s a tmpdir/sl
ln -s x tmpdir/dsl
mkdir tmpdir/sub
touch tmpdir/sub/x
mkdir tmpdir/sub2
chmod 0 tmpdir/sub2
printf "%s\n" "nftw_dir_tree tmpdir"
nftw_dir_tree tmpdir
printf "\n"
printf "%s\n" "nftw_dir_tree -d -p tmpdir"
nftw_dir_tree -d -p tmpdir
rm -rf tmpdir