<?php
	ob_start();
	session_start();
	require_once 'dbconnect.php';	
	
	// if session is not set this will redirect to login page
	if(!isset($_SESSION['user'])) {
		header("Location: index.php");
		exit;
	}
	$query = "INSERT INTO `utilizza` (`punteggio`, `Gio_IDGiocatore`, `Mod_IDModalita`) VALUES (0,".($_SESSION['user']).", 24)";
	$conn->query($query);
	$query = "SELECT `nr_partita` FROM `utilizza` ORDER BY `nr_partita` DESC LIMIT 1";
	$result = $conn->query($query);
	$row = $result->fetch_assoc();
				
	$_SESSION['game'] = $row['nr_partita'];
	// select loggedin users detail
	$query = "SELECT * FROM `giocatore` WHERE `Gio_ID`=".$_SESSION['user'];
	$result = $conn->query($query);
	$row = $result->fetch_assoc();
	$_SESSION['Nome'] = $row['Gio_Nome'];		
	$_SESSION['Cognome'] = $row['Gio_Cognome'];
	
?>
	
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Welcome - <?php echo $row['Gio_Email']; ?></title>
<link rel="stylesheet" href="assets/css/bootstrap.min.css" type="text/css"  />
<link rel="stylesheet" href="style.css" type="text/css" />
<link href="main.css" rel="stylesheet" type="text/css">
</head>
<body>

	<nav class="navbar navbar-default navbar-fixed-top">
      <div class="container">
        <div class="navbar-header">
          <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="https://github.com/ErikStallviere/ReactionGame2.0">ReactionGame</a>
        </div>
        <div id="navbar" class="navbar-collapse collapse">
          <ul class="nav navbar-nav navbar-right">
            <li class="dropdown">
              <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="false">
			  <span class="glyphicon glyphicon-user"></span>&nbsp;<?php echo $row['Gio_Email']; ?>&nbsp;<span class="caret"></span></a>
              <ul class="dropdown-menu">
				<li><a href="home.php"><span class="glyphicon glyphicon-log-out"></span>&nbsp;Gioca</a></li>
                <li><a href="logout.php?logout"><span class="glyphicon glyphicon-log-out"></span>&nbsp;Esci</a></li>
              </ul>
            </li>
          </ul>
        </div><!--/.nav-collapse -->
      </div>
    </nav> 
	    <script src="assets/jquery-1.11.3-jquery.min.js"></script>
    <script src="assets/js/bootstrap.min.js"></script>
	<div id="wrapper">
	<table>
	<tr>
	<td>
	<p>Scegli la modalità</p>
	</td>
	<form method="POST" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF']);?>">
	<td>
	<select name="mod" value="1">
		<?php
			for($i = 1; $i < 24; $i++):
		?>
			<option><?php echo $i?></option>
		<?php
			endfor;
		?>
	</select>
	</td>
	<td>
	<input type="submit" value="Vai alla Classifica!">
	</td>
	</form>
	</tr>
<?php 
		if($_SERVER["REQUEST_METHOD"] === "POST"){
		$currentMod = $_POST['mod'];
		echo "<h1>Modalità $currentMod</h1>";
		$_SESSION['game'] = 0;
		require_once 'classifica.php';
		}
	?>
	</div>
	</body>
</html>