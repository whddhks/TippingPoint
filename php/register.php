
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="https://fonts.googleapis.com/css2?family=Nanum+Gothic&display=swap" rel="stylesheet">
    <script src="https://kit.fontawesome.com/c89961a32e.js" crossorigin="anonymous"></script>
    <title>Document</title>
    <link rel="stylesheet" type="text/css" href="car_web.css">
</head>
<body>
<nav class="navbar">
        <div class="navbar__logo">
            <i class="fa-solid fa-car"></i>
            <a href="" id="navbar__logo">Carmanage_TippingPoint</a>
        </div>
    </nav>
    <form action="" method="POST">
        <div class = "inputBox">
            <div class="p">
                <h3>REGISTER</h3>
                    <div class="id">
                        <input type="text" name="id" placeholder="사용하실 아이디를 입력하세요." required/><br>
                    </div>
                    <div class="pw">
                        <input type="password" name="pw" placeholder="사용하실 비밀번호를 입력하세요." required/><br><br>
                    </div>
                    <span>
                        <input type="submit" name = "login" id="login" value="JOIN!"/>  
                        &nbsp;
                        <!-- <a href="login.php" id="loc_page">LOGIN</a> -->
                    </span>

        </div>
    </form>
    <?php
        if(isset($_POST['id']) && isset($_POST['pw'])){
            $id = $_POST['id'];
            $pw = $_POST['pw'];

            $conn = mysqli_connect('3.39.65.57','root','multi123','car_manage');
            $sql = "INSERT INTO register values('$id' , '$pw');";
            
            if($result = mysqli_query($conn,$sql)){
                echo "<script>alert('회원가입 완료')</script>";
                header('Location: login.php');
            }else{
                 echo "<script>alert('이미 존재하는 사용자입니다.')</script>";
        }
        mysqli_close($conn);
        }    
    ?>
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