
#ifndef S_SD_FILE_DOWNLOAD_H
#define S_SD_FILE_DOWNLOAD_H

const char sd_file_download[] = 

R"=====(

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <title>File List</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f4f4f9;
      margin: 0;
      padding: 20px;
      height: 100vh;
      display: flex;
      justify-content: center;
      align-items: center;
      box-sizing: border-box;
    }

    .container {
      background: #fff;
      border-radius: 10px;
      box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
      padding: 20px;
      width: 90vw;
      max-width: 800px;
      max-height: 90vh;
      overflow-y: auto;
      box-sizing: border-box;
      display: flex;
      flex-direction: column;
      align-items: center;
    }

    h2 {
      font-size: 42px;
      color: #000000;
      margin-bottom: 20px;
      text-align: center;
    }

    table {
      width: 100%;
      border-collapse: collapse;
      table-layout: fixed;
    }

    th, td {
      border: 2px solid #333;
      padding: 20px 10px;
      text-align: center;
      font-size: 24px;
      word-wrap: break-word;
    }

    th {
      background-color: #f2f2f2;
    }

    tr:nth-child(even) {
      background-color: #f9f9f9;
    }

    .action-btn {
      padding: 8px 12px;
      font-size: 18px;
      margin: 2px;
      cursor: pointer;
      border-radius: 4px;
      border: none;
    }

    .download-btn {
      background-color: #2196F3;
      color: white;
    }

    .download-btn:hover {
      background-color: #1976D2;
    }

    .delete-btn {
      background-color: #f44336;
      color: white;
    }

    .delete-btn:hover {
      background-color: #d32f2f;
    }

    .back-btn {
      margin-top: 20px;
      padding: 12px 20px;
      background-color: #4CAF50;
      color: white;
      border: none;
      font-size: 32px;
      width: 100%;
      border-radius: 6px;
      cursor: pointer;
      font-weight: bold;
    }

    .back-btn:hover {
      background-color: #388E3C;
    }
  </style>
</head>
<body>

<div class="container">
  <h2>File List</h2>

  <table>
    <thead>
      <tr>
        <th>Filename</th>
        <th>Download</th>
        <th>Delete</th>
      </tr>
    </thead>
    <tbody id="fileTableBody">
      <!-- Rows will be inserted here dynamically -->
    </tbody>
  </table>

  <button class="back-btn" onclick="goBack()">Back</button>
</div>

<script>
  // Fetch and display the file list
  function read_sd_file() {
    fetch("/sd_file")
      .then(response => {
        if (!response.ok) {
          throw new Error("HTTP error: " + response.status);
        }
        return response.json();  // parse response as JSON array
      })
      .then(data => {
        if (!Array.isArray(data) || data.length === 0) {
          throw new Error("Empty or invalid file list");
        }

        console.log("File List:", data);

        const tableBody = document.getElementById("fileTableBody");
        tableBody.innerHTML = "";  // Clear existing rows

        // Filter out unwanted files (case-insensitive)
        const filenames = data.filter(file => file.trim().toLowerCase() !== "system volume information");

        // Add a row for each filename
        filenames.forEach(file => {
          const row = document.createElement("tr");

          // Filename cell
          const fileCell = document.createElement("td");
          fileCell.textContent = file;
          row.appendChild(fileCell);

          // Download cell with button
          const downloadCell = document.createElement("td");
          const downloadBtn = document.createElement("button");
          downloadBtn.className = "action-btn download-btn";
          downloadBtn.textContent = "Download";
          downloadBtn.onclick = () => downloadFile(file);
          downloadCell.appendChild(downloadBtn);
          row.appendChild(downloadCell);

          // Delete cell with button
          const deleteCell = document.createElement("td");
          const deleteBtn = document.createElement("button");
          deleteBtn.className = "action-btn delete-btn";
          deleteBtn.textContent = "Delete";
          deleteBtn.onclick = () => deleteFile(file);
          deleteCell.appendChild(deleteBtn);
          row.appendChild(deleteCell);

          tableBody.appendChild(row);
        });
      })
      .catch(error => {
        console.error("Error fetching file list:", error);
        alert("Failed to load file list: " + error.message);
      });
  }

  // Download file handler
  function downloadFile(filename) {
    window.location.href = `/download?file=${encodeURIComponent(filename)}`;
  }

  // Delete file handler
  function deleteFile(filename) {
    if (confirm(`Are you sure you want to delete "${filename}"?`)) {
      fetch(`/delete_file?file=${encodeURIComponent(filename)}`, { method: 'DELETE' })
        .then(response => {
          if (response.ok) {
            alert(`"${filename}" deleted successfully.`);
            read_sd_file();  // Refresh file list
          } else {
            alert(`Failed to delete "${filename}".`);
          }
        })
        .catch(error => {
          console.error('Error deleting file:', error);
          alert(`Error deleting "${filename}": ${error.message}`);
        });
    }
  }

  // Go back handler
  function goBack() {
    window.history.back();
  }

  // Initial load and periodic refresh
  window.onload = function () {
    read_sd_file();
    setInterval(read_sd_file, 60000);  // Refresh every 10 seconds
  };

</script>

</body>
</html>



)=====";

#endif