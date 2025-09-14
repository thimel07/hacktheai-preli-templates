# FastAPI

Simple library member API built with Python and FastAPI.

## Run with Docker
```bash
docker-compose up --build
```

## Run locally
```bash
pip install -r requirements.txt
python main.py
```

## API Endpoints

- `POST /api/members` - Create a new member
- `GET /api/members` - Get all members
- `GET /api/members/{member_id}` - Get specific member
- `GET /docs` - Interactive API documentation

## Example Request
```bash
curl -X POST http://localhost:8000/api/members \
  -H "Content-Type: application/json" \
  -d '{"member_id": 1, "name": "Alice", "age": 22}'
```
