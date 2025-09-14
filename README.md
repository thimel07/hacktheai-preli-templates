# HackTheAI Preliminaries Templates

This repository contains template projects for different programming languages and frameworks to help you get started quickly with your HackTheAI preliminaries.

## Available Templates

- **Node.js Express** (`node_express/`) - A simple Express.js server with Docker setup
- **Python Django** (`py_django/`) - A Django REST API with library management system
- **Python FastAPI** (`py_fast_api/`) - A FastAPI application with Docker configuration

## Getting Started

Since this repository contains multiple examples, you can clone the entire repository and then copy only the specific template you need.

### Step 1: Clone the Repository

```bash
git clone https://github.com/yourusername/hacktheai-preli-templates.git
cd hacktheai-preli-templates
```

### Step 2: Copy the Template You Need

#### For Node.js Express:
```bash
cp -r node_express ../my-express-project
cd ../my-express-project
```

#### For Python Django:
```bash
cp -r py_django ../my-django-project
cd ../my-django-project
```

#### For Python FastAPI:
```bash
cp -r py_fast_api ../my-fastapi-project
cd ../my-fastapi-project
```

## Quick Start

After cloning your desired template:

1. Navigate to the template folder
2. Follow the README instructions in each template folder
3. Start building your HackTheAI preliminaries project!

#### Validating your submission

Creating a zip:
```bash
git archive -o your_team_handle.zip HEAD
```

Validating the zip:
```bash
unzip your_team_handle.zip -d your_team_handle
cd your_team_handle
docker compose up -d --build
```
Note: Replace `your_team_handle` with your actual team handle provided.

Validate API Endpoints:
```bash
curl -X POST http://localhost:8000/api/members \
  -H "Content-Type: application/json" \
  -d '{"member_id": 1, "name": "Alice", "age": 22}'
```

Once you are satisfied that your zip file is valid, submit it via instructed form. 