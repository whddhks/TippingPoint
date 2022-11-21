<?php
$conn = mysqli_connect(
  '3.39.65.57',
  'root',
  'multi123',
  'car_manage');

?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <script src="https://kit.fontawesome.com/c89961a32e.js" crossorigin="anonymous"></script>
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Nanum+Gothic&display=swap" rel="stylesheet">
    <title>Total Information</title>
    <script src="https://kit.fontawesome.com/551f0b3d13.js" crossorigin="anonymous"></script>

</head>

    <body>
        <nav class="navbar">
            <div class="navbar__logo">
                <i class="fa-solid fa-car"></i>
                <a href="">Carmanage_TippingPoint</a>
            </div>

            <ul class="navbar__menu">
                <li><a href="total.php">Total</a></li>
                <li><a href="illegal.php">Illegal</a></li>
                <li><a href="illegal_elec.php">Elec_illegal</a></li>
                <li><a href="intro.php">intro</a></li>
                <li><a href="extro.php">extro</a></li>
            </ul>

            <ul class="navbar__icons">
            
            </ul>

            <a href="#" class="navbar__tooglebtn">
                <i class="fa-solid fa-bars"></i>
            </a>
        </nav>
        <br>
        <h1 align = center>Total Information</h1>
        <br>
        <table id = 'box' border="1" width = 80% align = center>
            <tr>
                <td id = 'title' style ="border-radius: 10px 0 0 0;">first_number</td><td id = 'title'>last_number</td><td id = 'title'>person_type</td><td id = 'title'>car_elec</td><td id = 'title' style ="border-radius: 0 10px 0 0;">intro_time</td>
                <?php
                $sql = "SELECT * FROM total_info";
                $result = mysqli_query($conn, $sql);
                while($row = mysqli_fetch_array($result)){
                    $filtered = array(
                        'first_number'=>htmlspecialchars($row['first_number']),
                        'last_number'=>htmlspecialchars($row['last_number']),
                        'person_type'=>htmlspecialchars($row['person_type']),
                        'car_elec'=>htmlspecialchars($row['car_elec']),
                        'intro_time'=>htmlspecialchars($row['intro_time'])
                    );

                    ?>

                    <tr>
                        <td id = "fill"><?=$filtered['first_number']?></td>
                        <td id = "fill"><?=$filtered['last_number']?></td>
                        <td id = "fill"><?=$filtered['person_type']?></td>
                        <td id = "fill"><?=$filtered['car_elec']?></td>
                        <td id = "fill"><?=$filtered['intro_time']?></td>
                    </tr> 
                    <?php
                }
                ?>
            </tr>
        </table>
        <footer>
        &copy; copyright by TippingPoint<br>
        <i class="fa-solid fa-people-group"></i>&ensp;
        한소예&ensp;박나현&ensp;박지혜&ensp;유종완&ensp;정수현&ensp;
        <i class="fa-solid fa-people-group"></i><br>
        <i class="fa-solid fa-up-right-from-square"></i>
        <a href="https://github.com/whddhks/TippingPoint" target="_blank" style='color:azure'>GITHUB</a>&emsp;
        <i class="fa-solid fa-up-right-from-square"></i>
        <a href="https://www.notion.so/4-Tipping_Point-da6a5db42e9249d392be02ce6166981e" target="_blank" style='color:azure'>NOTION</a>
        </footer>
    </body>
</html>
<!-- echo "<h1>multi row</h1>";


$sql = "SELECT * FROM total_info";
$result = mysqli_query($conn, $sql);
while($row = mysqli_fetch_array($result)) {
    echo '<h2>'.$row['first_number']."  ".$row['last_number'].'</h2>';
  
} -->