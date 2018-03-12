<?php
$myFile = 'programas.txt';
$myFileLink = fopen($myFile, 'r');
$myFileContents = fread($myFileLink, filesize($myFile));
fclose($myFileLink);
echo $myFileContents;
?>
