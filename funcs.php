<?php
function numtochk($numero){
    
    $len = strlen($numero);
    $pts = [];
    while($len > 4){
        
        $pts[] = substr($numero, $len - 4, 4);
        $len -= 4;
    }
    $pts[] = substr($numero, 0, $len);
    return $pts;    
}

function multiplylong($numero1, $numero2){

    $pts1 = numtochk($numero1);
    $pts2 = numtochk($numero2);

    $sums = [];
    for($i = 0; $i < count($pts1); $i++){
        
        for($j = 0; $j < count($pts2); $j++){
            
            if(!isset($sums[$i+$j])){

                $sums[$i+$j] = $pts1[$i]*$pts2[$j];
            }
            else{
                $sums[$i+$j] += $pts1[$i]*$pts2[$j];
            } 
        }
    }
    $carry = 0;
    $str = "";
    for($i = 0; $i < count($sums); $i++){
        
        $val = $sums[$i] + $carry;
        $mod = $val % 10000;
        $carry = (int)($val/10000);
        $str = sprintf("%04d", $mod).$str;
    }

    if($carry != 0)
        $str = ltrim($carry, "0").$str;
    return $str;
}

function sumlong($numero1, $numero2){
    
    $au = [];
    $pts1 = numtochk($numero1);
    $pts2 = numtochk($numero2);
    if(count($pts1) < count($pts2)){
        
        $au = $pts2;
        $pts2 = $pts1;
        $pts1 = $au;
    }

    $result = [];
    $carry = 0;
    for($i = 0; $i < count($pts1); $i++){
        
        echo $i.", ".$carry."\n";
        if(!isset($pts2[$i])){
            $val2 = 0;
        }
        else{
            
            $val2 = $pts2[$i];
        }
    
        $val = $pts1[$i] + $val2 + $carry;
        $result[] = $val%10000;
        echo $val."\n";
        $carry = (int)($val/10000);
    }
    $result[] = $carry;
    $str = "";
    
    for($i = 0; $i < count($result); $i++){
        
        $str = sprintf("%04d", $result[$i]).$str;
    }
    return $str;
}
?>