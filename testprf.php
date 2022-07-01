<?php
$secret = "segredoooooooooooooooooooooooooo";
$seed = "sementeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
$label = "labellllll";
$key = hash_pbkdf2("sha384", $secret, $seed, 2, $label);
echo "out:\n".bin2hex($key)."\n";
?>