<?php
echo '<head>
  <link href="main.css" rel="stylesheet" type="text/css">
 </head>';
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "reactiongame";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
	die("Connessione fallita: " . $conn->connect_error);
} 
/*
echo "<select id='mod'>";
for($i = 0; $i < 23; $i++){
	echo "<option value='$i'>$i</option>";
	
}
echo "</select>";
*/
if(isset($currentMod) && isset($_SESSION['user'])){
	
//$sql = "SELECT Gio_Nome, punteggio FROM giocatore, utilizza, modalita WHERE Gio_ID = Gio_IDGiocatore AND Mod_ID = Mod_IDModalita";
$sql = "SELECT Gio_Nome, Gio_Cognome, punteggio, nr_partita FROM giocatore, utilizza WHERE Gio_ID = Gio_IDGiocatore AND Mod_IDModalita = $currentMod order by punteggio desc";
$result = $conn->query($sql);

echo "Tabella punteggi: <br>";
echo "<table class='table'>";

echo "<tr><th>Posizione</th><th>Nome</th><th>Cognome</th><th>Punteggio</th></tr>";

if ($result->num_rows > 0) {
	$count = 0;
    while($row = $result->fetch_assoc()) {
		$count++;
		if($_SESSION['Nome'] == $row["Gio_Nome"] && $_SESSION['Cognome'] == $row["Gio_Cognome"]){
			/*echo "correct";
			echo "$count";
			echo "<br>";*/
			$color = "#44E359";
			if($_SESSION['game'] == $row["nr_partita"]){
			$color = "#F64747";
			}
		}else{
			/*echo ($_SESSION['Nome']." ". $_SESSION['Cognome']);
			echo "  $count";
			echo ($row["Gio_Nome"]." ". $row["Gio_Cognome"]);
			echo "<br>";*/
			$color = "#F6FAF6";
		}
		//echo "$count $color <br>";
		
        echo "<tr bgcolor='$color'><td>".$count."</td><td>" . $row["Gio_Nome"]. "</td><td>" . $row["Gio_Cognome"]. "</td><td>". $row["punteggio"] . "</td></tr>";
    }
} else {
    echo "0 results";
}

$conn->close();
}
?>