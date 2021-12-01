# Define data structures and data structure items

# Event Object
class Event:
   def __init__(self, event_type=1, event_time=0, nextEvent=None):
      self.type = event_type
      self.time = event_time
      self.next = nextEvent
   
   def get_time(self):
      return self.time
   
   def get_next_event(self):
      return self.next 
   
   def get_event_type(self):
      return self.type
   
   def set_next_event(self, new_event):
      self.next = new_event
   
# EventQueue Object
class EventQueue:
   def __init__(self):
      self.head = None

   def get_head(self):
      return self.head

   def set_head(self, new_event):
      self.head = new_event

   # Insert into spot basedo on time (FCFS)
   def add_event(self, new_event):
      current = self.head

      if current is None:
         n = Event(new_event.type, new_event.time, new_event.next)
         self.head = n
         return
      
      while current.next is not None:
         if current.next.time > new_event.time:
            break
         current = current.next

      new_event.next = current.next
      current.next = new_event
      return
   
   # Remove event with highest priority (head, smallest time)
   def remove_event(self):
      if self.head.next is not None:
         to_remove = self.head
         self.head = self.head.next
         del to_remove
      else:
         to_remove = self.head
         self.head = None
         del to_remove