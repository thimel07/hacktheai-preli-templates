# Django

Simple library member API built with Python and Django REST Framework.

## Run with Docker
```bash
docker-compose up --build
```

## Run locally
```bash
pip install -r requirements.txt
python manage.py runserver
```

## API Endpoints

- `POST /api/members` or `POST /api/members/` - Create a new member
- `GET /api/members/list/` - Get all members
- `GET /api/members/{member_id}/` - Get specific member

## Example Request
```bash
curl -X POST http://localhost:8000/api/members \
  -H "Content-Type: application/json" \
  -d '{"member_id": 1, "name": "Alice", "age": 22}'
```

## Features

- Django REST Framework for API functionality
- In-memory storage (no database required)
- Input validation and error handling
- Minimal setup with only essential files
