@echo OFF 

:: Need to change this in order to get support for åäöÅÄÖ...
chcp 65001



sed -e "1s/^/[\n/" -e "$s/,$/\n]/" -e "s/\\xE5/å/g" -e "s/\\xE4/ä/g" -e "s/\\xF6/ö/g" -e "s/\\xC5/Å/g" -e "s/\\xC4/Ä/g" -e "s/\\xD6/Ö/g" temp.json > compile_commands.json	

:: powershell -Command sed -e '1s/^/[\n/' -e '$s/,$/\n]/' -e 's/\\xE5/å/g' -e 's/\\xE4/ä/g' -e 's/\\xF6/\xF6/' temp.json | out-file compile_commands.json -encoding oem

::powershell -Command "sed -e '1s/^/[\n/' -e '$s/,$/\n]/' -e 's/\\xE5/å/g' -e 's/\\xE4/ä/g' -e 's/\\xF6/\xF6/' temp.json | out-file compile_commands.json -encoding oem"

chcp %cp%>nul