<?php
$servername = "localhost:3306";
$username=$_GET['name'];
$password=$_GET['password'];
$database=$_GET['database'];
$licence_plate=$_GET['licence_plate'];
$licence_name=$_GET['licence_name'];
 
 
//连接数据库
try {
 
$conn = new PDO("mysql:host=$servername;dbname=$database", $username, $password);
 
 
$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
 
echo "连接成功";
 
}
 
catch(PDOException $e){
 
echo "连接失败:".$e->getMessage();
 
}
 
$sql = "INSERT INTO park(licence_plate,licence_name) VALUES ('".$licence_plate."','".$licence_name."');";
if($conn->query($sql)===TRUE){
    echo "succeed";
 
}
else{
    "Error:".$conn->error;
}
$conn->close();
?>