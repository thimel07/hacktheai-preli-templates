from fastapi import FastAPI, HTTPException
from pydantic import BaseModel, Field
from typing import Dict, List
import uvicorn

app = FastAPI(title="Library Member API", version="1.0.0")

# In-memory storage
members: Dict[int, dict] = {}

class MemberCreate(BaseModel):
    member_id: int = Field(..., description="Unique member ID")
    name: str = Field(..., description="Member name")
    age: int = Field(..., ge=0, le=150, description="Member age (0-150)")

class MemberResponse(BaseModel):
    member_id: int
    name: str
    age: int
    has_borrowed: bool

@app.post("/api/members", response_model=MemberResponse, status_code=201)
async def create_member(member: MemberCreate):
    """Create a new member in the library system"""
    
    # Check for duplicate ID
    if member.member_id in members:
        raise HTTPException(
            status_code=409,
            detail=f"member with id: {member.member_id} already exists"
        )
    
    # Create new member
    new_member = {
        "member_id": member.member_id,
        "name": member.name,
        "age": member.age,
        "has_borrowed": False
    }
    
    members[member.member_id] = new_member
    
    return new_member

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
