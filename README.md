cd /d/git/CV_00_Introduction
git filter-repo --to-subdirectory-filter CV_00_Introduction

cd /d/git/MSc

git remote add 0 https://github.com/wowCharming/ANLA_03_GE_CompletePivoting.git
git fetch 0 --tags
git merge --allow-unrelated-histories 0/master 
git remote remove 0
mkdir -p ANLA/ANLA_03_GE_CompletePivoting

#manually do
#git mv file1 file2 folder1/* PTfS/PTfS_01/
#git mv -k * PTfS/PTfS_01/

git add .
git commit -m "Move files into ANLA subdirectory"
git push origin main
