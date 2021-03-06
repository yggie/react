#include "react/Dynamics/ContactGraph.h"

#include "react/Collision/Shapes/shapes.h"

using namespace re;

const reUInt LIMIT = 10;

/// NOT TESTED
ContactEdge::ContactEdge(reAllocator& allocator, Entity& a, Entity& b) : re::Intersect(), A(a), B(b), contact(false), timeLimit(0), interactions(allocator) {
  check();
}

/// NOT TESTED
void ContactEdge::check() {
  if (timeLimit++ == 0) {
    timeLimit = LIMIT;
  }

  contact = re::intersects(A.shape(), A.transform(), B.shape(), B.transform(), *this);
}

/// NOT TESTED
ContactGraph::ContactGraph(reAllocator& allocator) : _allocator(allocator), _edges(allocator) {
  // do nothing
}

/// NOT TESTED
ContactGraph::~ContactGraph() {
  for (ContactEdge* edge : _edges) {
    for (reInteraction* action : edge->interactions) {
      _allocator.alloc_delete(action);
    }
    _allocator.alloc_delete(edge);
  }
  _edges.clear();
}

/// NOT TESTED
void ContactGraph::solve() {
  const reFloat epsilon = 0.9;
  // TODO TEMPORARY
  for (ContactEdge* edge : _edges) {
    if (edge->contact) {
      Entity& A = edge->A;
      Entity& B = edge->B;
      
      const re::vec3 dA = edge->point - A.center();
      const re::vec3 rA = dA - re::dot(dA, edge->normal) * dA;
      const re::vec3 dB = edge->point - B.center();
      const re::vec3 rB = dB - re::dot(dB, edge->normal) * dB;
      const re::vec3 rAxN = re::cross(rA, edge->normal);
      const re::vec3 rBxN = re::cross(rB, edge->normal);
      
      const re::vec3 inerA = A.inertiaInv() * rAxN;
      const re::vec3 inerB = B.inertiaInv() * rBxN;
      
      const reFloat numer = -(1 + epsilon) *
                            (re::dot(edge->normal, A.vel() - B.vel()) +
                            (re::dot(A.angVel(), rAxN) - re::dot(B.angVel(), rBxN)));
      
      // contact forces must always be repelling
      if (numer < 0.0) continue;
      
      const reFloat f = numer / (A.massInv() + B.massInv() + re::dot(rAxN, inerA) + re::dot(rBxN, inerB));
      
      const re::vec3 impulse = f * edge->normal;
      
      A.addImpulse(impulse);
      B.addImpulse(-impulse);
//      A.applyRotImpulse(impulse * rAxN);
//      B.applyRotImpulse(impulse * inerB);
    }
    for (reInteraction* action : edge->interactions) {
      action->solve(edge->A, edge->B);
    }
  }
}

/// NOT TESTED
void ContactGraph::check(Entity& A, Entity& B) {
  // sanity check
  RE_ASSERT(A.id() < B.id());
  
  // higher level grouping filters
  if (!_filter.filter((const Entity&)A, (const Entity&)B)) return;
  
  for (ContactEdge* edge : _edges) {
    if (edge->A.id() == A.id() && edge->B.id() == B.id()) {
      edge->check();
      return;
    }
  }
  
  // the edge does not exist, create a new one
  ContactEdge* edge = _allocator.alloc_new<ContactEdge>(_allocator, A, B);
  _edges.add(edge);
}

/// NOT TESTED
void ContactGraph::advance() {
  reLinkedList<ContactEdge*> toRemove(_allocator);
  // checks for rejected edges
  for (ContactEdge* edge : _edges) {
    if (edge->timeLimit != 0) edge->timeLimit--;
    if (edge->timeLimit == 0 && edge->interactions.empty()) {
      toRemove.add(edge);
    }
  }
  
  // removes all rejected edges
  for (ContactEdge* edge : toRemove) {
    _edges.remove(edge);
    _allocator.alloc_delete(edge);
  }
}

/// NOT TESTED
void ContactGraph::addInteraction(reInteraction& action, Entity& A, Entity& B) {
  if (A.id() > B.id()) {
    addInteraction(action, B, A);
    return;
  }
  
  for (ContactEdge* edge : _edges) {
    if (edge->A.id() == A.id() && edge->B.id() == B.id()) {
      edge->interactions.add(&action);
      return;
    }
  }
  
  // the edge does not exist, create a new one
  ContactEdge* edge = _allocator.alloc_new<ContactEdge>(_allocator, A, B);
  edge->timeLimit = 0;
  edge->interactions.add(&action);
  _edges.add(edge);
}

