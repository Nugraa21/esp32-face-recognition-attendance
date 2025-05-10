<?php
include 'config.php';
$id = $_GET['id'];
$query = mysqli_query($conn, "SELECT * FROM users WHERE id=$id");
$data = mysqli_fetch_assoc($query);

if (isset($_POST['submit'])) {
    $name = $_POST['name'];
    if ($_FILES['photo']['name']) {
        $photo = $_FILES['photo']['name'];
        $tmp = $_FILES['photo']['tmp_name'];
        move_uploaded_file($tmp, "uploads/".$photo);
        mysqli_query($conn, "UPDATE users SET name='$name', photo='$photo' WHERE id=$id");
    } else {
        mysqli_query($conn, "UPDATE users SET name='$name' WHERE id=$id");
    }
    header("Location: data_users.php");
}
?>
<h2>Edit User</h2>
<form method="post" enctype="multipart/form-data">
    Nama: <input type="text" name="name" value="<?php echo $data['name']; ?>"><br>
    Foto: <input type="file" name="photo"><br>
    <img src="uploads/<?php echo $data['photo']; ?>" width="150"><br>
    <button type="submit" name="submit">Update</button>
</form>
