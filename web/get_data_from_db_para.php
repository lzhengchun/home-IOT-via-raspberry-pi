<?php
if (isset($argv)) {
    $latest_min = $argv[1];
}
else
{
    $latest_min = $_GET["latest_min"];
}
$con = mysql_connect("localhost","your user name","your password");

if (!$con) {
  die('Could not connect: ' . mysql_error());
}

mysql_select_db("home_status", $con);

$result = mysql_query("SELECT * FROM humidity_and_temperature WHERE humidity_and_temperature.date_time > DATE_SUB(now(), INTERVAL ".$latest_min." MINUTE);");

while($row = mysql_fetch_array($result)) {
    echo $row['date_time'] . "\t" . $row['humidity']. "\t" . $row['temperature']. "\n";
}

mysql_close($con);
?>
