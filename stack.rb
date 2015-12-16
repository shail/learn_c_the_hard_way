class Stack
  def initialize(initial = [])
    @stack = initial
  end

  def push(value)
    @stack << value
  end

  def pop()
    @stack.pop
  end
end

stack = Stack.new
stack.push(1)
stack.push(3)
stack.push(5)

puts stack.pop == 5
puts stack.pop == 3
puts stack.pop == 1
