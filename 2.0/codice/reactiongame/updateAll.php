<?php
$score = $_GET['score'];
$modality = $_GET['modality'];

$servername = "localhost";
$username = "root";
$password = "";
//$password = "root1";
$dbname = "reactiongame";


$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
	die("Connessione fallita: " . $conn->connect_error);
}
$query = "SELECT `Mod_IDModalita`, `Gio_IDGiocatore` FROM `utilizza` ORDER BY `nr_partita` DESC LIMIT 1";


$result = $conn->query($query);
$row = $result->fetch_assoc();
echo "<pre>";
print_r($row);
echo "</pre>";
if($row['Mod_IDModalita'] != 24){
	$query = "INSERT INTO `utilizza` (`punteggio`, `Gio_IDGiocatore`, `Mod_IDModalita`) VALUES (0,".($row['Gio_IDGiocatore']).", 24)";
	$conn->query($query);
}
$sql = "SELECT `nr_partita` FROM `utilizza` ORDER BY `nr_partita` DESC LIMIT 1";
$result = $conn->query($sql);
$row = $result->fetch_assoc();
echo $row['nr_partita'];
$sql = "UPDATE `utilizza` SET `Mod_IDModalita` = ".$modality." WHERE `nr_partita` = ".$row['nr_partita'];

$sql2 = "UPDATE `utilizza` SET `punteggio` = ".$score." WHERE `nr_partita` = ".$row['nr_partita'];
$result = $conn->query($sql);

$result = $conn->query($sql2);

$conn->close();


?>