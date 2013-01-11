#!/bin/bash -e


VOTCASHARE="$(csg_call --show-share)"
calculators="$(ctp_run --list | sed -ne 's/^\s\+\([a-z]*\)\s*\(.*\)/\1/p')"
texfile="$PWD/calculators.tex"

add_heads() {
  local i head pattern again="no"
  for i in $items; do
    head=${i%.*}
    #for main node
    [ "$head" = "$i" ] && continue
    pattern=${head//$/\\$}
    #if head node is note there
    if [ -z "$(echo -e "$items" | grep -Ee "$pattern([[:space:]]+|\$)")" ]; then
      items="$(echo -e "$items\n$head")"
      again="yes"
      #echo "added $head from $i xx $pattern" >&2
      #break here to avoid double head nodes
      break
    fi
  done
  #we have to do that because items were changed
  [ "$again" = "yes" ] && add_heads
}

cut_heads() {
  local i new 
  [ -z "$1" ] && die "cut_heads: Missing argument"
  item="$1"
  hspace=0
  spaces=""
  for ((i=0;i<5;i++)); do
    new="${item#*.}"
    [ "$new" = "$item" ] && break
    item="$new"
    spaces+="  "
    let "hspace+=10"
  done
}

# get the loop for all calculators
echo calculators: $calculators
rm -f $texfile

for calculator in ${calculators}; do
  xmlfile=${VOTCASHARE}/ctp/xml/$calculator.xml


  if [ ! -f "$xmlfile" ]; then 
    continue
  fi

  calculator_description="$(csg_property --file $xmlfile --path $calculator --print description --short)"
  calculator_sectionlabel="$(csg_property --file $xmlfile --path $calculator --print sectionlabel --short)"

  echo "\subsection{$calculator}" >> $texfile
  echo "\label{calc:$calculator}" | sed  -e 's/\_/\\_/g' >> $texfile
  echo "%" >> $texfile
  echo $calculator_description"." | sed -e 's/\_/\\_/g' >> $texfile

  items="$(csg_property --file $xmlfile --path $calculator.item --print name --short)" || die "parsing xml failed"

  if [ -n "$items" ]; then
    echo % >> $texfile; 
    echo "\rowcolors{1}{invisiblegray}{white}" >> $texfile
    echo -e "\n "$calculator [sec:$calculator_sectionlabel]: $calculator_description"\n   properties: "$items
    echo "{ \small" >> $texfile
    echo "\begin{longtable}{m{3cm}|m{11cm}}" >> $texfile

    for name in ${items}; do  
      cut_heads "$name"
      head="$(echo $name | sed -e 's/'${item}'//')"
      description="$(csg_property --file $xmlfile --path $calculator.item --filter "name=$name" --print description --short)" || die "${0##*/}: Could not get desc for $name"
      item="$(echo $item | sed -e 's/\_/\\_/g')"
      description="$(echo $description | sed -e 's/\_/\\_/g')"
      #echo ITEM $item
      #echo "      "$head $name $item $description
      echo " \hspace{${hspace}pt} \hypertarget{$calculator.${trunc}${name}}{${item}}  & ${description} \\\\" >> $texfile
    done

    echo  \\end{longtable} >> $texfile
    echo "}" >> $texfile
    echo "%" >> $texfile 
  fi
  if [ -n "$calculator_sectionlabel" ]; then
    echo "\noindent Return to the description of \slink{$calculator_sectionlabel}{\texttt{$calculator}}." >> $texfile
  fi
  echo >> $texfile

done




