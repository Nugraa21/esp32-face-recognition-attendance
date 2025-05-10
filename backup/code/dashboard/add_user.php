<?php include 'config.php'; ?>
<?php
if (isset($_POST['submit'])) {
    $name = $_POST['name'];
    $photo = $_FILES['photo']['name'];
    $tmp = $_FILES['photo']['tmp_name'];
    move_uploaded_file($tmp, "uploads/".$photo);
    mysqli_query($conn, "INSERT INTO users (name, photo) VALUES ('$name', '$photo')");
    header("Location: data_users.php");
}
?>
<h2>Tambah User</h2>
<form method="post" enctype="multipart/form-data">
    Nama: <input type="text" name="name" required><br>
    Foto: <input type="file" name="photo" required><br>
    <button type="submit" name="submit">Simpan</button>
</form>
