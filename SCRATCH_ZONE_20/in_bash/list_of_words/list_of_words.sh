LIST=(
	echo
#	echo is the command
	one
#	one
	two three
	four
)

for i in 4 1 2 0 3 @
do
	echo "pick word $i ${LIST[$i]}"
done
	echo "pick word '@' ${LIST[@]}"

	"${LIST[@]}" "# call the command with added words"
