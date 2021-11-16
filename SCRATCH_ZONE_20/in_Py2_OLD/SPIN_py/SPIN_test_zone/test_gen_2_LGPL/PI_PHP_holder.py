
class PI_PHP_holder:
	"""
		PI_PHP_holder is how a piece of PHP is held in the tree,
		and how it is printed.

		SGML_tree_printer calls INSTANCE.tree_print( printer )
		and privides the write() function to output data
		(but doesnt do any wrapping on the data)
	"""
	def __init__( self, text="" ):
		self.text = text

	def tree_print( self, tree ):
		tree.write( '<?php ' )
		tree.write( self.text )
		tree.write( ' ?>' )

	def add( self, text ):
		self.text = self.text + text
