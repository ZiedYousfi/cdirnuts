#!/usr/bin/env lua

-- Simple example: Create a basic project structure

print("Creating a simple web project...\n")

-- Create root directory
local project = cdirnuts.allocDir("./my_web_project")

-- Create subdirectories
local publicDir = cdirnuts.allocDir("./my_web_project/public")
local cssDir = cdirnuts.allocDir("./my_web_project/public/css")
local jsDir = cdirnuts.allocDir("./my_web_project/public/js")

-- Create HTML file
local indexHtml = cdirnuts.createFile(
    "./my_web_project/public/index.html",
    [[<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>My Web Project</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <h1>Welcome to My Web Project</h1>
    <p>Created with cdirnuts!</p>
    <script src="js/app.js"></script>
</body>
</html>
]])

-- Create CSS file
local cssFile = cdirnuts.createFile(
    "./my_web_project/public/css/style.css",
    [[body {
    font-family: Arial, sans-serif;
    max-width: 800px;
    margin: 50px auto;
    padding: 20px;
    background-color: #f5f5f5;
}

h1 {
    color: #333;
}
]])

-- Create JS file
local jsFile = cdirnuts.createFile(
    "./my_web_project/public/js/app.js",
    [[console.log('Web project loaded successfully!');

document.addEventListener('DOMContentLoaded', function() {
    console.log('DOM fully loaded');
});
]])

-- Create README
local readme = cdirnuts.createFile(
    "./my_web_project/README.md",
    "# My Web Project\n\nA simple web project created with cdirnuts Lua API.\n"
)

-- Build the structure
cdirnuts.addFileToDir(cssDir, cssFile)
cdirnuts.addFileToDir(jsDir, jsFile)
cdirnuts.addFileToDir(publicDir, indexHtml)
cdirnuts.addFileToDir(project, readme)

cdirnuts.addSubDirToDir(publicDir, cssDir)
cdirnuts.addSubDirToDir(publicDir, jsDir)
cdirnuts.addSubDirToDir(project, publicDir)

-- Create everything on disk
if cdirnuts.createDir(project) then
    print("✓ Project created successfully!")
    print("\nProject structure:")
    cdirnuts.executeCommand("tree ./my_web_project 2>/dev/null || ls -R ./my_web_project")
else
    print("✗ Failed to create project")
end
