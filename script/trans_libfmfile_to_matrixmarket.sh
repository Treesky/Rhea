#!/bin/sh

libfmfile=heart_scale

max_fea_idx=`cat $libfmfile | awk '{split($0,tokens," ");for(i=2;i<=length(tokens);i++){split(tokens[i],ts,":");if(ts[1]>max){max=ts[1]}}}END{print max+1}'`

total_atom_num=`cat $libfmfile | awk '{split($0, tokens, " "); total_atom_num+=(length(tokens)-1); }END{print total_atom_num;}'`

total_ins_num=`cat $libfmfile | awk '{}END{print NR}'`

echo $max_fea_idx, $total_atom_num, $total_ins_num

cat $libfmfile | awk -v max_fea_idx=$max_fea_idx -v total_atom_num=$total_atom_num -v total_ins_num=$total_ins_num '
BEGIN{
    print "%%MatrixMarket matrix coordinate real general";
    print total_ins_num,max_fea_idx,total_atom_num;
}
{
    split($0,tokens," ");
    for(i=2; i<=length(tokens); i++)
    {
        split(tokens[i], ts, ":");
        print NR,ts[1],ts[2]
    }
}' > feature_file

cat $libfmfile | awk -v total_ins_num=$total_ins_num '
BEGIN{
    print "%%MatrixMarket matrix array real general"
    print total_ins_num, 1, total_ins_num;
}
{
    split($0,tokens," ");
    print tokens[1];
}' > label_file
