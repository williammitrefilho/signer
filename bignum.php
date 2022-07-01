<?php
require "funcs.php";

if($argc < 3)
    exit("nao informado");

$numero1 = preg_replace("/[^0-9]/", "", $argv[1]);
$strs = "";
for($i = 2; $i < $argc; $i++){
    
    $numero2 = preg_replace("/[^0-9]/", "", $argv[$i]);
    $str = multiplylong($numero1, $numero2);
    $strs .= $numero1." * ".$numero2." = ".$str."\n\n";
    $numero1 = $str;
}
echo $strs;
$arq = fopen("lognum.log", "a");
fwrite($arq, $strs);
fclose($arq);
?>