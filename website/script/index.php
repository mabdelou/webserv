

<style>
.mycss{
    width: 100%;
    padding: 12px;
    border: 1px solid #ccc;
    border-radius: 4px;
    box-sizing: border-box;
    resize: vertical;
}

h2{
      font-family: Verdana, Geneva, Tahoma, sans-serif;
}
</style>

<?php
    if ( isset( $_POST['submit'] ) ) { 
        $firstname = $_POST['firstname'];
        $lastname = $_POST['lastname'];
        echo '<h2>Form POST Method</h2>';
        echo "<p class='mycss'>Your name is ";
        echo $firstname . ' ' . $lastname;
        exit;
    }
    else{

        $firstname = $_GET['firstname'];
        $lastname = $_GET['lastname'];
        echo '<h2>Form GET Method</h2>';
        echo "<p class='mycss'>Your name is ";
        echo $firstname . ' ' . $lastname;
        exit;
    }
?>