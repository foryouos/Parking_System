echo "Start auto deploy "
git add .
read -p "请输入commit 信息:   " title
git commit -m "$title"
git push

echo "Finish"
echo 按任意键继续
read -n 1