<!DOCTYPE html>
<!--
   Statement of Authorship:
   I, Ricardo Solis solisr 400460236, certify that this material is my original work.
   No other person's work has been used without due acknowledgement.
   I have not made my work available to anyone else.
-->
<?php


/**
 * The function used to connect to the database, run the query and given the result
 * @param $query
 * @return bool|mysqli_result|void
 * You need to provide your credentials.php file in the same folder with this php script
 * Or provide the variables: $host, $user, $password, $dbname in this function
 */
function runQuery($query)
{
    require "credentials.php";


    //connect of database else display error
    $link = mysqli_connect($host, $user, $password);


    if (!$link) {
        $status = 'Could not connect: ' . mysqli_connect_error();
        echo $status;
        die();
    }


    mysqli_select_db($link, $dbname);


    $result = mysqli_query($link, $query) or die('Query failed: ' . mysqli_error($link));


    $link->close();
    return $result;


}


// This is the function to create table header
// By calling this function, a table header with the text and soring icon will be displayed
// By clicking the sorting icon, the parameter orderby and order will be sent to the URL
// You can get the parameter orderby and order's value by calling $_GET global array
// you don't need to change this function
function createJoinHeader()
{
    echo "<tr>
           <th>First Name
           <a href='$_SERVER[PHP_SELF]?orderby=FirstName&order=asc'>" . iconUp() . "</a>
           <a href='$_SERVER[PHP_SELF]?orderby=FirstName&order=desc'>" . iconDown() . "</a>
           </th>
           <th>Last Name
           <a href='$_SERVER[PHP_SELF]?orderby=LastName&order=asc'>" . iconUp() . "</a>
           <a href='$_SERVER[PHP_SELF]?orderby=LastName&order=desc'>" . iconDown() . "</a>
           </th>
           <th>Title
           <a href='$_SERVER[PHP_SELF]?orderby=Title&order=asc'>" . iconUp() . "</a>
           <a href='$_SERVER[PHP_SELF]?orderby=Title&order=desc'>" . iconDown() . "</a>
           </th>
           <th>Orders
           <a href='$_SERVER[PHP_SELF]?orderby=Orders&order=asc'>" . iconUp() . "</a>
           <a href='$_SERVER[PHP_SELF]?orderby=Orders&order=desc'>" . iconDown() . "</a>
           </th>
           <th>Total Sales
           <a href='$_SERVER[PHP_SELF]?orderby=TotalSales&order=asc'>" . iconUp() . "</a>
           <a href='$_SERVER[PHP_SELF]?orderby=TotalSales&order=desc'>" . iconDown() . "</a>
           </th>
           <th>Average Sales
           <a href='$_SERVER[PHP_SELF]?orderby=AverageSales&order=asc'>" . iconUp() . "</a>
           <a href='$_SERVER[PHP_SELF]?orderby=AverageSales&order=desc'>" . iconDown() . "</a>
           </th>
       </tr>
";
}


// The function used to create the sorting up button
// You don't need to change this
function iconUp()
{
    return "<svg xmlns='http://www.w3.org/2000/svg' width='16' height='16' fill='currentColor' class='bi bi-caret-up-square' viewBox='0 0 16 16'>
         <path d='M14 1a1 1 0 0 1 1 1v12a1 1 0 0 1-1 1H2a1 1 0 0 1-1-1V2a1 1 0 0 1 1-1h12zM2 0a2 2 0 0 0-2 2v12a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V2a2 2 0 0 0-2-2H2z'/>
         <path d='M3.544 10.705A.5.5 0 0 0 4 11h8a.5.5 0 0 0 .374-.832l-4-4.5a.5.5 0 0 0-.748 0l-4 4.5a.5.5 0 0 0-.082.537z'/>
         </svg>";
}


// The function used to create the sorting down button
// You don't need to change this
function iconDown()
{
    return "<svg xmlns='http://www.w3.org/2000/svg' width='16' height='16' fill='currentColor' class='bi bi-caret-down-square' viewBox='0 0 16 16'>
         <path d='M3.626 6.832A.5.5 0 0 1 4 6h8a.5.5 0 0 1 .374.832l-4 4.5a.5.5 0 0 1-.748 0l-4-4.5z'/>
         <path d='M0 2a2 2 0 0 1 2-2h12a2 2 0 0 1 2 2v12a2 2 0 0 1-2 2H2a2 2 0 0 1-2-2V2zm15 0a1 1 0 0 0-1-1H2a1 1 0 0 0-1 1v12a1 1 0 0 0 1 1h12a1 1 0 0 0 1-1V2z'/>
         </svg>";
}


// The function used to create a table body
// By passing the mysqli_fetch_all() function result to this function
// It will return the database tables with HTML format
// You don't need to change this
function createTableBody($dataArr)
{
    for($i=0; $i<count($dataArr); $i++)
    {
        echo "<tr>";
        for($j=0; $j<count($dataArr[$i]); $j++)
        {
            printf("<td>%s</td>",$dataArr[$i][$j]);
        }
    }
}


?>


<html lang="en">
<head>
    <!--HTML BoilerPlate Content" -->
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="Ricardo Solis - solisr">
    <meta name="author" content="Ricardo Solis">
    <meta keywords="Ricardo Solis - solisr, PROCTECH 4IT3, SEP 6IT3, McMaster University, HTML, CSS, JavaScript">
    <meta name="robots" content="index, follow">


    <!--
    BoilerPlate items for Social Media Robots
    -->
    <meta property="og:title" content="Ricardo Solis - solisr - Ricardo Solis"/>
    <meta property="og:type:article:author" content="Ricardo Solis"/>
    <meta property="og:type:article:section" content="Ricardo Solis - solisr"/>
    <meta property="og:url" content="https://4it3.sept.mcmaster.ca/solisr/FinalExam/final.php"/>


    <title>Ricardo Solis - solisr</title>


    <!--
      BoilerPlate BOOTSTRAP!
   -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js"></script>
    <style>
        .centerd {
            align-items: center;
            align-content: center;
            text-align: center;
        }
    </style>
</head>
<header>
    <div class="header mt-auto py-3 bg-dark centerd fixed-top">
        <div class="container text-white">
            <h1> 2026 Final Exam </h1>
        </div>
    </div>
    <br/><br/><br/><br/>
</header>
<body>


<?php


// This is a basic select query for the database
// You can provide your query in the later section based on this basic version
$query = "SELECT employees.FirstName, employees.LastName, employees.Title,
               COUNT(orders.OrderID) AS Orders,
               CAST(SUM(products.UnitPrice) AS DECIMAL(10,2)) AS TotalSales,
               CAST(AVG(order_details.Quantity * order_details.UnitPrice) AS DECIMAL(6,2)) AS AverageSales
       FROM products
       INNER JOIN order_details ON order_details.ProductID = products.ProductID
       INNER JOIN orders ON orders.OrderID = order_details.OrderID
       INNER JOIN employees ON employees.EmployeeID = orders.EmployeeID
       GROUP BY employees.FirstName, employees.LastName, employees.Title
       ORDER BY SUM(products.UnitPrice) DESC;";


// Run the query and get the result
$data = runQuery($query);
// The data type of the $data variable is an associate array
$data = mysqli_fetch_all($data);


// Build title list from initial data
$titles = array();
for ($i = 0; $i < count($data); $i++) {
    $titles[] = $data[$i][2];
}
$titles = array_unique($titles);
sort($titles);


// Find max total sales from initial data
$maxSales = max(array_column($data, 4));


// Read current GET values
$currentTitle = "";
if (isset($_GET['Title'])) {
    $currentTitle = $_GET['Title'];
}


$currentMax = $maxSales;
if (isset($_GET['TotalSalesMax']) && $_GET['TotalSalesMax'] !== "") {
    $currentMax = $_GET['TotalSalesMax'];
}


// Read sort values
$orderBy = "TotalSales";
if (isset($_GET['orderby'])) {
    $orderBy = $_GET['orderby'];
}


$order = "DESC";
if (isset($_GET['order']) && strtolower($_GET['order']) == "asc") {
    $order = "ASC";
}


// Keep sorting limited to valid displayed columns
switch ($orderBy) {
    case "FirstName":
    case "LastName":
    case "Title":
    case "Orders":
    case "TotalSales":
    case "AverageSales":
        break;
    default:
        $orderBy = "TotalSales";
}


// Build dynamic query
$query = "SELECT employees.FirstName, employees.LastName, employees.Title,
               COUNT(orders.OrderID) AS Orders,
               CAST(SUM(products.UnitPrice) AS DECIMAL(10,2)) AS TotalSales,
               CAST(AVG(order_details.Quantity * order_details.UnitPrice) AS DECIMAL(6,2)) AS AverageSales
       FROM products
       INNER JOIN order_details ON order_details.ProductID = products.ProductID
       INNER JOIN orders ON orders.OrderID = order_details.OrderID
       INNER JOIN employees ON employees.EmployeeID = orders.EmployeeID";


if ($currentTitle != "") {
    $query .= " WHERE employees.Title='$currentTitle'";
}


$query .= " GROUP BY employees.FirstName, employees.LastName, employees.Title";


if (is_numeric($currentMax)) {
    $query .= " HAVING CAST(SUM(products.UnitPrice) AS DECIMAL(10,2)) <= $currentMax";
}


$query .= " ORDER BY $orderBy $order";


// Run dynamic query
$data = runQuery($query);
$data = mysqli_fetch_all($data);


?>


<!-- HTML Form used to collect user input data -->
<div class="w-50 mx-auto">
    <form method="get" action="<?= $_SERVER['PHP_SELF'] ?>"> <!--ACTION?-->
        <div>
            <h3>Filter</h3>
            <div class="row g-3">
                <div class="col">
                    <select name="Title" class="form-select" aria-label="Titles">
                        <option value="">Title</option>


                        <?php
                        // TODO
                        // Based on the query result you should build the select option list
                        // The value of the option list should be the value of the column Title
                        for ($i = 0; $i < count($titles); $i++) {
                            $selected = "";
                            if ($currentTitle == $titles[$i]) {
                                $selected = "selected";
                            }
                            printf("<option value=\"%s\" %s>%s</option>", $titles[$i], $selected, $titles[$i]);
                        }
                        ?>


                    </select>
                </div>
                <div class="col">
                    <label for="TotalSalesMax" class="form-label">Total Sales Range: 0 to <?= number_format((float)$maxSales, 2, '.', '') ?>. Current Value:&nbsp;</label>
                    <output><?= $currentMax ?></output>
                    <input name="TotalSalesMax" type="range" class="form-range" min="0" step="0.01"
                           value="<?= $currentMax ?>"
                           max="<?= $maxSales ?>" id="TotalSalesMax" oninput="this.previousElementSibling.value = this.value">
                </div>
            </div>
            <button class="btn btn-primary float-end">Filter</button>
        </div>
    </form>


</div>
<br/><hr/>


<!-- HTML Table to present data from database -->
<div class="w-75 mx-auto">
    <table class="table">
        <thead class="thead table-light">
        <?php createJoinHeader() ?>
        </thead>
        <tbody>
        <?php
        createTableBody($data);
        ?>
        </tbody>
    </table>
</div>
</body>
<br/><br/><br/>
<footer class="footer mt-auto py-3 bg-dark centerd fixed-bottom">
    <div class="container">
        <span class="text-muted"> &copy; Ricardo Solis 2026</span>
    </div>
</footer>


</html>
