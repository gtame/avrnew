

<?php

$myFile = $_GET["file"];
$saved=false;

if (isset($_POST["content"])) 
{
	$myFileLink2 = fopen($myFile, 'w+') or die("Can't open file.");
	$newContents = $_POST["content"];
	$oldlen=strlen($newContents);
	$newlen=0;
	while ($oldlen!=$newlen)
	{
		$oldlen=strlen($newContents);
		$newContents = str_replace("\r\n\r\n", "\r\n", $newContents);
		$newlen=strlen($newContents);
	}

	fwrite($myFileLink2, $newContents);
	fclose($myFileLink2);
	$saved=true;
}

$myFileLink = fopen($myFile, 'r');
$myFileContents = fread($myFileLink, filesize($myFile));
fclose($myFileLink);


?>

<html>
<title>Config. riegos v1.0</title>
<head></head>
<body>
<?php if ($saved) { ?>
<h2>Guardado con exito</h2>
<?php 
 } 
 if ($myFile=="config.txt") { 
?>
<pre>
Config
==========
	//12665331679911111215
	//3 para AvisosSMS
	//6 siguientes para movil
	//NUM 4 password sms
	 //BOOL 1 o 0 para motor diesel
	//NUM 1 para puertos de abono
	//NUM 2 para puertos (sectores)
</pre>
<?php 
}
if ($myFile=="programas.txt") { 
?>
<pre>
Programas
==========
//SSDDDHHMMRRRRAAAA
//SS   -> Sector
//DDD  -> Dias que se ejecutara (enum)
//HHMM -> Hora inicio ó Ejecución en el otro bloque
//RRRR -> Tiempo de riego
//AAAA -> Tiempo de abono

//Ej, 03101010120001200000
//03->Sector
//0101010-> Dias que se ejecutara (Martes,Jueves,Sabado)
//1200 -> Se ejecutara a las 12:00
//0120 -> Tiempo de riego
//0000 -> Tiempo de abono
</pre>
<?php } ?>

<form name="form" action="" method="post">
	<textarea name="content" rows="10" cols="30"><?php echo $myFileContents;?></textarea><br/>
  <input type="submit" value="Submit">
</form>

</body>
</html>