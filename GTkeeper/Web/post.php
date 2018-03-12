<?php
// En versiones de PHP anteriores a la 4.1.0, debería utilizarse $HTTP_POST_FILES en lugar
// de $_FILES.
//error_reporting(E_ALL);

if (isset($_GET['SN']))
{

	$dir_subida = $_SERVER["DOCUMENT_ROOT"]."/custom/files/";
	$path_file=$dir_subida.date("YmdHis")."_".$_GET['SN'].".dat";
	
	if (move_uploaded_file($_FILES['submitted']['tmp_name'], $path_file)) 
	{
		echo "OK";
	}
	else 
	{
	    echo "ERR";
	    echo "El fichero -> ".$_FILES['submitted']['tmp_name'];
	}
	
	//Si queremos debugear..
	//print_r($_FILES);
	

}
else
{
?>


<html>
<body>
<form action="http://posttestserver.com/post.php?dir=example&SN=asdas" method="post" enctype="multipart/form-data">
File: <input type="file" name="submitted">
<input type="hidden" name="someParam" value="someValue"/>
<input type="submit" value="send">
</form>



<form action="http://clubciclistautebo.es/custom/post.php?SN=123123123" method="post" enctype="multipart/form-data">
File: <input type="file" name="submitted">
<input type="hidden" name="someParam" value="someValue"/>
<input type="submit" value="send">
</form>
</body>
</html>

<?php 
}
?>