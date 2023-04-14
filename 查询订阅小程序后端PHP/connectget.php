<?php
header('Content-type: application/json; charset=utf-8');

$servername = "localhost:3306";
$username=$_GET['name'];
$password=$_GET['password'];
$database=$_GET['database'];

//连接数据库
try {
    $conn = new PDO("mysql:host=$servername;dbname=$database;charset=utf8", $username, $password);

    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // Query the database for parking lot names and available spots
    $sql = "SELECT P_name, P_all_count - P_reserve_count - P_now_count AS surplus FROM parking;";
    $result = $conn->query($sql);

    // Create an empty array to store the parking lot information
    $parking_lots = [];

    // Loop through the query result and populate the array with parking lot names and available spots
    while ($row = $result->fetch(PDO::FETCH_ASSOC)) {
        $parking_lot = [
            "place_names" => $row["P_name"],
            "place_number" => $row["surplus"]
        ];
        array_push($parking_lots, $parking_lot);
    }

    // Convert the array to a JSON object and encode it
    $json_output = json_encode($parking_lots);
    $json_output = urlencode($json_output);

    // Send the JSON object to the frontend
    echo $json_output;
} catch(PDOException $e) {
    echo "连接失败:".$e->getMessage();
}
?>
