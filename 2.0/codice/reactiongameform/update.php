<?php 
	ob_start();
	session_start();
	require_once 'dbconnect.php';
	$query = "SELECT `nr_partita` FROM `utilizza` ORDER BY `nr_partita` DESC LIMIT 1";
	$result = $conn->query($query);
	$row = $result->fetch_assoc();				
	$_SESSION['game'] = $row['nr_partita'];
	
	$query = "SELECT * FROM `utilizza` WHERE `Gio_IDGiocatore`=".$_SESSION['user']." AND `nr_partita`=".$_SESSION['game'];
	$result = $conn->query($query);
	$row = $result->fetch_assoc();
	$currentMod = null;
	
	if($row['Mod_IDModalita'] != 24 && isset($row['Mod_IDModalita'])) {
		$currentMod = $row['Mod_IDModalita'];
		$currentPoints = $row['punteggio'];
		echo '<p style="font-size : 59px;">Complimenti '.$_SESSION['Nome'].'! Giocando alla modalità <b>'.$currentMod.'</b>, hai totalizzato un punteggio di <b>'.$currentPoints.'</b></p>';	
	}
	require_once "classifica.php";
?>