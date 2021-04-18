#!/bin/sh

#-------------------------------------------------------------------------------------
#   Common variable:
#-------------------------------------------------------------------------------------
uppercase_flag=0
lowercase_flag=0
recursive_flag=0
sed_pattern_flag=0

uppercase_sed="s/[a-z]/\U&/g"
lowercase_sed="s/[A-Z]/\L&/g"
pattern_sed=""


#-------------------------------------------------------------------------------------
help_text() {
#-------------------------------------------------------------------------------------
    echo "Usage:
        $0 [-r] [-l|-u] <dir/file names...>
        $0 [-r] <sed pattern> <dir/file names...>
        $0 [-h]
                    
Explenation:
        -l lowercase given filename
        -u uppercase given filename
        <sed pattern> sed pattern of given filename
        -r recursive modification for files udner given dictonary
        "
    exit 2
}


#-------------------------------------------------------------------------------------
recursive(){
#-------------------------------------------------------------------------------------
    if [ -d $1 ]; then
        cd ./$1;
        for file in $(ls)
        do
            recursive $file $2
        done
        cd ..
    else
        change_filename $1 $2
    fi

}


#-------------------------------------------------------------------------------------
change_filename(){
#-------------------------------------------------------------------------------------
    dirname=$(dirname "$1")
    filename=$(basename "$1" | cut -f 1 -d '.')
    extension=$(echo "$1" | cut -f 2 -d '.')
    # echo $dirname
    # echo $filename
    # echo $extension

    if [ $1 = $extension ]; then
        extension=''
    fi

    case $2 in
        "-u") filename=$(echo "$filename" | sed -e "$uppercase_sed") ;;
        "-l") filename=$(echo "$filename" | sed -e "$lowercase_sed") ;;
        "-s") filename=$(echo "$filename" | sed -e "$pattern_sed") ;;
    esac

    if [ -z "$extension" ]; then
        mv "$1" "$dirname/$filename";
    else
        mv "$1" "$dirname/$filename.$extension";
    fi
}


#-------------------------------------------------------------------------------------
select_method(){
#-------------------------------------------------------------------------------------   
    if [ ! -f $1 ] && [ ! -d $1 ]; then
        echo "\nInvalid argument. Given file/directory is incorrect"; help_text ; return ;
    fi
    
    if [ $uppercase_flag -eq 1 ] && [ $lowercase_flag -eq 1 ]; then
        echo "It is not allow to define -u and -l argument"
        help_text
        exit 2
    fi

    if [ $uppercase_flag -eq 0 ] && [ $lowercase_flag -eq 0 ] && [ $sed_pattern_flag -eq 0 ]; then
        echo "Please define flag!"
        help_text
        exit 2
    fi
    
    if [ $recursive_flag -eq 1 ]; then
        if [ $uppercase_flag -eq 1 ]; then
            recursive $1 -u
            
        fi
    
        if [ $lowercase_flag -eq 1 ]; then
            recursive $1 -l
        fi 

        if [ $sed_pattern_flag -eq 1 ]; then
            recursive $1 -s
        fi 
    
    else
    
        if [ $uppercase_flag -eq 1 ]; then
            change_filename $1 -u
        fi
    
        if [ $lowercase_flag -eq 1 ]; then
            change_filename $1 -l
        fi
        
        if [ $sed_pattern_flag -eq 1 ]; then
            change_filename $1 -s
        fi 
    fi
}


#-------------------------------------------------------------------------------------
#   main
#-------------------------------------------------------------------------------------
while [ "$1" != "" ];
do
    case $1 in
        "-u" | "-uppercase" | "-U")     uppercase_flag=1 ;;
        "-l" | "-lowercase" | "-L")     lowercase_flag=1 ;;
        "-r" | "-recursive" | "-R")     recursive_flag=1 ;;
        "-h" | "-help" | "-H")          help_text ;;
        -*)                             echo "Uknown paramter!"; help_text ;;

        *)  if [ $uppercase_flag -ne 1 ] && [ $lowercase_flag -ne 1 ] && [ ! -d $1 ] && [ ! -f $1 ] && [ $sed_pattern_flag -ne 1 ]   ; then
                sed_pattern_flag=1
                pattern_sed=$1
            else
                select_method $1
            fi ;;
    esac
    shift 1;
done