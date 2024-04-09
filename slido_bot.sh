#!/bin/bash
# slido voting bot
echo -e "\e[1;34m——————————No voting?—————————————\e[0m"
echo -e "\e[1;34m⠀⣞⢽⢪⢣⢣⢣⢫⡺⡵⣝⡮⣗⢷⢽⢽⢽⣮⡷⡽⣜⣜⢮⢺⣜⢷⢽⢝⡽⣝   \e[0m"
echo -e "\e[1;34m⠸⡸⠜⠕⠕⠁⢁⢇⢏⢽⢺⣪⡳⡝⣎⣏⢯⢞⡿⣟⣷⣳⢯⡷⣽⢽⢯⣳⣫⠇   \e[0m"
echo -e "\e[1;34m⠀⠀⢀⢀⢄⢬⢪⡪⡎⣆⡈⠚⠜⠕⠇⠗⠝⢕⢯⢫⣞⣯⣿⣻⡽⣏⢗⣗⠏⠀   \e[0m"
echo -e "\e[1;34m⠀⠪⡪⡪⣪⢪⢺⢸⢢⢓⢆⢤⢀⠀⠀⠀⠀⠈⢊⢞⡾⣿⡯⣏⢮⠷⠁⠀⠀    \e[0m"
echo -e "\e[1;34m⠀⠀⠀⠈⠊⠆⡃⠕⢕⢇⢇⢇⢇⢇⢏⢎⢎⢆⢄⠀⢑⣽⣿⢝⠲⠉⠀⠀⠀⠀   \e[0m"
echo -e "\e[1;34m⠀⠀⠀⠀⠀⡿⠂🇺🇦⡇⢇⠕⢈⣀⠀⠁⠡⠣⡣⡫⣂⣿⠯⢪⠰⠂⠀⠀⠀⠀   \e[0m"
echo -e "\e[1;34m⠀⠀⠀⠀⡦⡙⡂⢀⢤⢣⠣⡈⣾⡃⠠🇺🇦⡄⢱⣌⣶⢏⢊⠂⠀⠀⠀⠀⠀⠀   \e[0m"
echo -e "\e[1;34m⠀⠀⠀⠀⢝⡲⣜⡮⡏⢎⢌⢂⠙⠢⠐⢀⢘⢵⣽⣿⡿⠁⠁⠀⠀⠀⠀⠀⠀⠀   \e[0m"
echo -e "\e[1;34m⠀⠀⠀⠀⠨⣺⡺⡕⡕⡱⡑⡆⡕⡅⡕⡜⡼⢽⡻⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀   \e[0m"
echo -e "\e[1;34m⠀⠀⠀⠀⣼⣳⣫⣾⣵⣗⡵⡱⡡⢣⢑⢕⢜⢕⡝⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀   \e[0m"
echo -e "\e[1;34m⠀⠀⠀⣴⣿⣾⣿⣿⣿⡿⡽⡑⢌⠪⡢⡣⣣⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀   \e[0m"
echo -e "\e[1;34m⠀⠀⠀⡟⡾⣿⢿⢿⢵⣽⣾⣼⣘⢸⢸⣞⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀   \e[0m"
echo -e "\e[1;34m⠀⠀⠀⠀⠁⠇⠡⠩⡫⢿⣝⡻⡮⣒⢽⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀   \e[0m"
echo -e "\e[1;34m—————————————————————————————————\e[0m"



# Help                                                     #
############################################################
Help()
{
   # Display Help
   echo "suck my dick slido"
   echo
   echo "Syntax: [-u|h|v]"
   echo "options:"
   echo "u     set slido url"
   echo "h     print this help."
   echo "v     vote question number"
   echo "V     how many votes"
}

############################################################
############################################################

# main program
while getopts ":hu:v:V:" option; do
   case $option in
      h) # display Help
         Help
         exit;;
      u) # Enter url
         url=$OPTARG;;
      v) # enter vote number
	 question_number=$OPTARG;;
      V) # enter how many votes
	 vote_number=$OPTARG;;
     \?) # Invalid option
         echo "Error: Invalid option"
         exit;;
   esac
done
if [[ $# -eq 0 ]]; then
    Help
    exit 0
fi
hash=$( echo $url | cut -d "/" -f 5 )
auth_tokens=()
event_uuid=$(curl -s https://app.sli.do/eu1/api/v0.5/app/events?hash=$hash | grep -oE '"uuid":"[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}"' | cut -c 9- | rev | cut -c2- | rev)
for ((c=1; c<=$vote_number; c++))
do
	auth_token=$(curl -s "https://app.sli.do/eu1/api/v0.5/events/"$event_uuid"/auth" --data '{"initialAppViewer":"browser--other","granted_consents":["StoreEssentialCookies"]}' | jq -r '.access_token')
	auth_tokens+=("$auth_token")
done
summary="curl -s \"https://app.sli.do/eu1/api/v0.5/events/$event_uuid/summary\" -H \"Authorization: Bearer "$auth_token"\""
summary_uuids=$( eval "$summary" )
activePollUuid=$( echo $summary_uuids | jq -r '.bySection | .[].activePollUuid' )
sectionUuid=$( echo $summary_uuids | jq -r '.bySection | keys[]' )
question="curl -s \"https://app.sli.do/eu1/api/v0.5/events/$event_uuid/polls-v2?sectionUuid=$sectionUuid&onlyActive=true\" -H \"Authorization: Bearer "$auth_token"\""
question_uuid=$( eval "$question" )
options_uuid=$(echo $question_uuid | jq -r '.[].questions[].options[].uuid')
feedback_question_uuid=$(echo $question_uuid | jq -r '.[].questions[].options[].pollQuestionUuid')
feedback_question_uuid=$(echo $feedback_question_uuid | awk '{print $1}')
feedback_question_option_uuid=$( echo $options_uuid | awk -v var="$question_number" '{print $var}')

for token in ${auth_tokens[@]}
do
clientId=$( openssl rand -base64 20 | tr -dc 'a-zA-Z0-9' | head -c 15 )
curl -s 'https://app.sli.do/eu1/api/v0.5/events/'$event_uuid'/polls/'$activePollUuid'/vote' \
  -H 'accept: application/json, text/plain, */*' \
  -H 'accept-language: en-GB,en;q=0.6' \
  -H 'authorization: Bearer '$token'' \
  -H 'content-type: application/json' \
  -H 'origin: https://app.sli.do' \
  -H 'referer: https://app.sli.do/event/'$hash'/live/polls' \
  -H 'sec-ch-ua: "Brave";v="123", "Not:A-Brand";v="8", "Chromium";v="123"' \
  -H 'sec-ch-ua-mobile: ?0' \
  -H 'sec-ch-ua-platform: "Windows"' \
  -H 'sec-fetch-dest: empty' \
  -H 'sec-fetch-mode: cors' \
  -H 'sec-fetch-site: same-origin' \
  -H 'sec-gpc: 1' \
  -H 'user-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36' \
  -H 'x-client-id: '$clientId'' \
  -H 'x-slidoapp-version: SlidoParticipantApp/53.200.4 (web)' \
  --data-raw '{"feedback_uuid":"'$activePollUuid'","votings":[{"feedback_question_uuid":"'$feedback_question_uuid'","feedback_question_option_uuid":"'$feedback_question_option_uuid'","is_anonymous":true}],"is_anonymous":true}' 1> /dev/null
done

echo -e "\e[1;34mFinished lmao\e[0m"
