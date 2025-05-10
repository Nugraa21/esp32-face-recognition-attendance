<?php include 'config.php'; ?>
<!DOCTYPE html>
<html>
<head>
    <title>Data Users</title>
</head>
<body>
    <h1>Data Users (Dataset)</h1>
    <a href="add_user.php">Tambah User</a>
    <table border="1">
        <tr>
            <th>No</th>
            <th>Nama</th>
            <th>Foto</th>
            <th>Action</th>
        </tr>
        <?php
        $no=1;
        $query = mysqli_query($conn, "SELECT * FROM users");
        while($row = mysqli_fetch_assoc($query)) {
            echo "<tr>
                <td>".$no++."</td>
                <td>".$row['name']."</td>
                <td><img src='uploads/".$row['photo']."' width='100'></td>
                <td>
                    <a href='edit_user.php?id=".$row['id']."'>Edit</a> | 
                    <a href='delete_user.php?id=".$row['id']."' onclick='return confirm(\"Yakin?\")'>Hapus</a>
                </td>
            </tr>";
        }
        ?>
    </table>
</body>
</html>
