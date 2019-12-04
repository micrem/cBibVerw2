<?php

$databasePDO = null;
/*
	function DBconnect();
	function DBend();
	function getEntries();
	function getEntryById($id);
	function newEntry($newEntryDataset);
	function modifyEntry($ID, $newEntryDataset);
	function deleteEntry($ID);
*/


/**
autodoc test
*/
function DBconnect(){
	global $useDB; //"global" gibt an dass Variable ausserhalb der Funktion existiert und verwendet werden soll
	global $databasePDO;
	if(!$useDB) return true;
	$user = "tagebuchUser";
	$pass = "YI6iqDwpvzMJMuAP";	
	try{
		$databasePDO = new PDO('mysql:host=localhost;dbname=db;charset=utf8', $user, $pass);
	}
	catch(PDOException $e)
    {
		echo "Konnte nicht zur Datenbank verbinden!<br>" . $e->getMessage()."<br>";
    }
	$databasePDO->setAttribute(PDO::ATTR_EMULATE_PREPARES, false); //soll Prepared Statements nicht nur emulieren sondern tatsächlich verwenden
	$databasePDO->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION); //soll Exceptions werfen statt Abbruch&Fehler 
	if (!$databasePDO) {return false;}
	else {return true;}
};

function DBend(){
	global $useDB;
	global $databasePDO;
	if(!$useDB) return;
	$databasePDO=null;
}

function getEntries($orderBy = 0){
	global $useDB;
	global $databasePDO;
	if(!$useDB) return getEntriesStatic();
	if (!$databasePDO) return null;
	if ($orderBy === -1){
		$sth = $databasePDO->query('SELECT * FROM tagebuch ORDER BY Datum ASC');
	}elseif ($orderBy === 1) {
		$sth = $databasePDO->query('SELECT * FROM tagebuch ORDER BY Datum DESC');
	}else{
		$sth = $databasePDO->query('SELECT * FROM tagebuch');
	}
	$result = $sth->fetchAll();
	return $result;
};

function getEntryById($id){
	global $useDB;
	global $databasePDO;
	if(!$useDB) return getEntryByIDStatic($id);
	if (!$databasePDO) return null;
	if (!is_numeric($id)) return null;
	$sth = $databasePDO->query("SELECT * FROM tagebuch WHERE ID=$id"); 
	$result = $sth->fetch();
	return $result;
}

function newEntry($newEntryDataset){
	global $useDB;
	global $databasePDO;
	if(!$useDB) return false;
	return false;
};

function modifyEntry($ID, $newEntryDataset){
	global $useDB;
	global $databasePDO;
	if(!$useDB) return false;
	
	
	
	return false;
};

function deleteEntry($ID){
	global $useDB;
	global $databasePDO;
	if(!$useDB) return false;
	return false;
}	


// Hilfsfunktionen
function getEntriesStatic(){
	return [
				[
					"ID"=>"6",
					"Datum"=>"2019-11-30",
					"Stunden"=>"1+4+6",
					"Beschreibung"=>"Vorlesung 123, Inhalt",
					"Fehlt"=>"alle da",
					"Spaet"=>"bernt, bob, bernadine",
					"Frueh"=>"lipshitz, loraine, hopital",
					"Unruhe"=>"diesersatzkeinverb",
					"Anmerkung"=>"China asshoe",
					"Hausaufgaben"=>"Fasten, 2 Monate",
					"Kommentar"=>"The beatings will continue until morale improves.",
					"Uebertragen"=>"vorbereitet"
				],
				[
					"ID"=>"7",
					"Datum"=>"2018-11-31",
					"Stunden"=>"5+6+7",
					"Beschreibung"=>"Vorlesung 123, Inhalt",
					"Fehlt"=>"Aalle da",
					"Spaet"=>"Abernt, bob, bernadine",
					"Frueh"=>"Alipshitz, loraine, hopital",
					"Unruhe"=>"Adiesersatzkeinverb",
					"Anmerkung"=>"China asshoe",
					"Hausaufgaben"=>"Fasten, 2 Monate",
					"Kommentar"=>"The beatings will continue until morale improves.",
					"Uebertragen"=>"ja"
				],
					[
					"ID"=>"8",
					"Datum"=>"2011-8-31",
					"Stunden"=>"5+7",
					"Beschreibung"=>"Vorles23, Inhalt",
					"Fehlt"=>"Aalle da",
					"Spaet"=>"Abernt, bob, bernadine",
					"Frueh"=>"Alipshitz, loraine, hopital",
					"Unruhe"=>"Adiesersatzkeinverb",
					"Anmerkung"=>"China asshoe",
					"Hausaufgaben"=>"Fasten, 2 Monate",
					"Kommentar"=>"The beatings will continue until morale improves.",
					"Uebertragen"=>"ja"
				],
									[
					"ID"=>"9",
					"Datum"=>"2010-8-31",
					"Stunden"=>"5+7+11",
					"Beschreibung"=>"Vorles2352453, Inhalt",
					"Fehlt"=>"Aalle das",
					"Spaet"=>"Abernt, blob, bernadine",
					"Frueh"=>"Alipshitz, loraine, hopital",
					"Unruhe"=>"Adiesersatzkeinverb",
					"Anmerkung"=>"Timeline? Time LINE?? Time is not made out of lines. It is made out of cirlces. That is why clocks are round.",
					"Hausaufgaben"=>"Fasten, 2 Monate",
					"Kommentar"=>"The beatings will continue until morale improves.",
					"Uebertragen"=>"nein"
				]
			];
}

function getEntryByIDStatic($ID){
	$ret=getEntriesStatic()[0];
	$ret["ID"]=$ID;
	return $ret;
}

?>
